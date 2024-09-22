
#include "Define_Structures.c"


void Admin_and_User() 
{
    Clinic clinic;
    initialize_clinic(&clinic);
    
    u32 mode;
    u32 attempts = 0;
    u32 password =1234;
    u32 entered_password;
    
    printf ("Clinic Management System\n");
    printf ("1-Admin mode:\n");
    printf ("2-User mode:\n") ;
    printf ("Choose mode : ");
    scanf ("%d", &mode);
    
    if (mode == 1) 
    { 
        while (attempts < MAX_ATTEMPTS) 
        {
            printf("Enter admin password: ");
            scanf("%d", &entered_password);
            
            if (entered_password == password) 
            {
                break;
            }
            
            attempts++;
            printf("Incorrect password. Attempts left: %d\n", MAX_ATTEMPTS - attempts);
        }
        
        if (attempts == MAX_ATTEMPTS) 
        {
            printf("Too many incorrect attempts. Exiting...\n");
            return 0;
        }
        
        u32 choice;

        do {
            printf("\nAdmin Menu:\n");
            printf("1. Add new patient record\n");
            printf("2. Edit patient record\n");
            printf("3. Reserve a slot with the doctor\n");
            printf("4. Cancel reservation\n");
            printf("5. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            
            switch (choice) 
            {
                case 1: add_patient_record(&clinic); break;
                case 2: edit_patient_record(&clinic); break;
                case 3: reserve_time_slot(&clinic); break;
                case 4: cancel_reservation(&clinic); break;
                case 5: printf("Exiting admin mode...\n"); break;
                default: printf("Invalid choice. Try again.\n");
            }
        } while (choice != 5);
        
    } 
    else if (mode == 2) 
    {
        u32 choice;

        do {
            printf("\nUser Menu:\n");
            printf("1. View patient record\n");
            printf("2. View today's reservations\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            
            switch (choice) 
            {
                case 1: view_patient_record(&clinic); break;
                case 2: view_todays_reservations(&clinic); break;
                case 3: printf("Exiting user mode...\n"); break;
                default: printf("Invalid choice. Try again.\n");
            }
        } while (choice != 3);
    } 
    else 
    {
        printf("Invalid mode selected.\n");
    }

}


void initialize_clinic(Clinic* clinic) 
{
    clinic->patients = NULL;
    clinic->reservations = NULL;
    
    for (u32 i = 0; i < SLOT_COUNT; i++) 
    {
        clinic->available_slots[i] = 1;
    }
}


void add_patient_record(Clinic* clinic) 
{
    Patient* new_patient = (Patient*)malloc(sizeof(Patient));

    printf("Enter patient name: ");
    scanf("%s", new_patient->name);

    printf("Enter patient age: ");
    scanf("%d", &new_patient->age);

    printf("Enter patient gender (M/F): ");
    scanf(" %c", &new_patient->gender);
    
    printf("Enter patient ID: ");
    scanf("%d", &new_patient->id);
    
    if (find_patient_by_id(clinic, new_patient->id) != NULL) 
    {
        printf("Patient ID already exists.\n");
        free(new_patient);
        return;
    }
    
    new_patient->next = clinic->patients;
    clinic->patients = new_patient;
    printf("Patient record added successfully.\n");
}


void edit_patient_record(Clinic* clinic) 
{
    u32 id;

    printf("Enter patient ID to edit: ");
    scanf("%d", &id);
    
    Patient* patient = find_patient_by_id(clinic, id);

    if (patient == NULL) 
    {
        printf("Patient ID not found.\n");
        return;
    }
    
    printf("Enter new name: ");
    scanf("%s", patient->name);

    printf("Enter new age: ");
    scanf("%d", &patient->age);

    printf("Enter new gender (M/F): ");
    scanf(" %c", &patient->gender);
    
    printf("Patient record updated successfully.\n");
}

void reserve_time_slot(Clinic* clinic) 
{
    print_available_slots(clinic);
    
    u32 id, slot;

    printf("Enter patient ID: ");
    scanf("%d", &id);
    
    if (find_patient_by_id(clinic, id) == NULL) 
    {
        printf("Patient ID not found.\n");
        return;
    }
    
    printf("Enter slot number to reserve (1-5): ");
    scanf("%d", &slot);
    
    if (slot < 1 || slot > SLOT_COUNT || clinic->available_slots[slot - 1] == 0) 
    {
        printf("Invalid or unavailable slot.\n");
        return;
    }
    
    Reservation* new_reservation = (Reservation*)malloc(sizeof(Reservation));
    new_reservation->patient_id = id;
    new_reservation->slot_index = slot - 1;
    new_reservation->next = clinic->reservations;
    clinic->reservations = new_reservation;
    
    clinic->available_slots[slot - 1] = 0; 
    printf("Slot reserved successfully.\n");
}


void cancel_reservation(Clinic* clinic) 
{
    u32 id;

    printf("Enter patient ID to cancel reservation: ");
    scanf("%d", &id);
    
    Reservation* prev = NULL;
    Reservation* current = clinic->reservations;
    

    while (current != NULL && current->patient_id != id) 
    {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) 
    {
        printf("No reservation found for this patient ID.\n");
        return;
    }
    
    if (prev == NULL) 
    {
        clinic->reservations = current->next;
    } 
    else 
    {
        prev->next = current->next;
    }
    
    clinic->available_slots[current->slot_index] = 1; 
    free(current);
    printf("Reservation cancelled successfully.\n");
}


void view_patient_record(Clinic* clinic) 
{
    u32 id;

    printf("Enter patient ID to view record: ");
    scanf("%d", &id);
    
    Patient* patient = find_patient_by_id(clinic, id);
    if (patient == NULL) 
    {
        printf("Patient ID not found.\n");
        return;
    }
    
    printf("Patient Name: %s\n", patient->name);
    printf("Patient Age: %d\n", patient->age);
    printf("Patient Gender: %c\n", patient->gender);
    printf("Patient ID: %d\n", patient->id);
}


void view_todays_reservations(Clinic* clinic) 
{
    printf("Today's Reservations:\n");

    for (u32 i = 0; i < SLOT_COUNT; i++) 
    {
        printf("Slot %d: ", i + 1);
        Reservation* current = clinic->reservations;

        u32 found = 0;
        
        while (current != NULL) 
        {
            if (current->slot_index == i) 
            {
                printf("Reserved by patient ID %d ", current->patient_id);
                found = 1;
            }
            current = current->next;
        }
        if (!found) 
        {
            printf("Available");
        }
        printf("\n");
    }
}


void print_available_slots(Clinic* clinic) 
{
    printf("Available Slots:\n");

    for (u32 i = 0; i < SLOT_COUNT; i++) 
    {
        if (clinic->available_slots[i]) 
        {
            printf("Slot %d: ", i + 1);
            switch (i) 
            {
                case 0: printf("2:00 PM - 2:30 PM\n"); break;
                case 1: printf("2:30 PM - 3:00 PM\n"); break;
                case 2: printf("3:00 PM - 3:30 PM\n"); break;
                case 3: printf("4:00 PM - 4:30 PM\n"); break;
                case 4: printf("4:30 PM - 5:00 PM\n"); break;
            }
        }
    }
}


Patient* find_patient_by_id(Clinic* clinic, u32 id) 
{
    Patient* current = clinic->patients;

    while (current != NULL) 
    {
        if (current->id == id) 
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
