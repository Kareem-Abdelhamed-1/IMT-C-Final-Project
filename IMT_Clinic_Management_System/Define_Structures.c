#include "STD_Types.h"
#include "BIT_MATH.h"


#define SLOT_COUNT 5
#define MAX_ATTEMPTS 3


typedef struct Patient 
{
    u8 name[100];
    u32 age;
    u8 gender;
    u32 id;
    struct Patient* next; 
} Patient;


typedef struct Reservation 
{
    u32 patient_id;
    u32 slot_index;
    struct Reservation* next; 
} Reservation;


typedef struct Clinic 
{
    Patient* patients;
    Reservation* reservations; 
    u32 available_slots[SLOT_COUNT]; 
} Clinic;


void add_patient_record(Clinic* clinic);
void edit_patient_record(Clinic* clinic);
void reserve_time_slot(Clinic* clinic);
void cancel_reservation(Clinic* clinic);
void view_patient_record(Clinic* clinic);
void view_todays_reservations(Clinic* clinic);
void print_available_slots(Clinic* clinic);
Patient* find_patient_by_id(Clinic* clinic, u32 id);
void initialize_clinic(Clinic* clinic);
