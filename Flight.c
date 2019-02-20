#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 

#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h> // The maximum number of passenger threads. 
#define MAX_PASSENGERS 10 // Function prototypes...
void *passenger(void *num); 
void *airplane(void *);  
//Define the semaphores. 
sem_t airplanecapacity; 
sem_t weight_luggage; 
sem_t check_luggage; 
sem_t get_pass; 
sem_t wait_boarding;
int allDone = 0; 

int main(int argc, char *argv[]) 
{
    pthread_t btid; 
    pthread_t tid[MAX_PASSENGERS]; 
    int i, x, numPassengers, numSeats; int Number[MAX_PASSENGERS]; 
    printf("Maximum number of Passengers can only be 10. \nEnter number of Passengers and seats.\n");
    scanf("%d",&x);
    numPassengers = x; 
    scanf("%d",&x);
    numSeats = x;
    if (numPassengers > MAX_PASSENGERS) { 
       printf("The maximum number of Passengers is %d.\n", MAX_PASSENGERS);    
       return 0;
    } 

printf("Boarding of Passengers using semaphores.\n"); 
    for (i = 0; i < MAX_PASSENGERS; i++) { 
        Number[i] = i; 
    } 

    // Initialize the semaphores with initial values... 
    sem_init(&airplanecapacity, 0, numSeats); 
    sem_init(&weight_luggage, 0, 1);
    sem_init(&check_luggage, 0, 0); 
    sem_init(&get_pass, 0, 0); 
    sem_init(&wait_boarding, 0, 0); 

    // Create the flight. 
    pthread_create(&btid, NULL, airplane, NULL); 

    // Create the passenger. 
    for (i = 0; i < numPassengers; i++) {
        pthread_create(&tid[i], NULL, passenger, (void *)&Number[i]); 
    } 

// Join each of the threads to wait for them to finish. 
    for (i = 0; i < numPassengers; i++) { 
        pthread_join(tid[i],NULL); 
    } 

allDone = 1; 
    sem_post(&wait_boarding); // Wait for boarding. 
    pthread_join(btid,NULL);
    printf("The Flight is ready to Take Off.\n"); 
    return 0;
} 

void *passenger(void *number) { 
     int num = *(int *)number;
     sem_wait(&weight_luggage);
     sleep(4);
     printf("Passenger %d weighing luggage.\n", num+1); 
     sem_post(&weight_luggage);

     sleep(7);
     printf("Passenger %d luggage is checked by security.\n", num+1); 
     sem_post(&check_luggage);

     sleep(3);
     printf("Passenger %d getting boarding pass.\n", num+1); 
     sem_post(&get_pass);

     printf("Passenger %d ready for boarding.\n", num+1); 
     printf("Passenger %d boarding the Airplane.\n", num+1);
     sem_wait(&check_luggage);
     sem_wait(&get_pass);
     //sem_wait(&wait_boarding);
     sem_post(&wait_boarding); 
     printf("---->Passenger %d boarded.\n", num+1); 
} 

void *airplane(void *junk) 
{    
  while (!allDone) {
    printf("The flight is waiting\n"); 
    sem_wait(&wait_boarding);    if (!allDone) 
    {
     printf("The passengers are still boarding\n"); 
     sleep(5);
     sem_post(&wait_boarding); 
    } 
    else { 
         printf("The flight has Departed.\n"); 
    } 
   }
}
