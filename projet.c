#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double a = -4; //start of the study zone
double b = 4; //end of the study zone
int N = 100; //number of creatures
int generation = 150; //number of generation
double PR = 0.5; //recombination probability
double PM = 0.01; //mutation probability
int doElitist = 0; //elitist mode(the best creature is replicated 5 times) 1 = yes, 0 = no

int *intToBin(int number); //convert a number to a string of binary number
int *intToBin2(int number);
int binToInt(int bits[]); //reverse convertion of the binary number
int reproduction(double fitness[], double random, double minFitness); //reproduction sequence (phase 1)
void recombination(double random, int particles[], int particle1, int particle2); //recomination sequence (phase 2)
void mutation(double random, int particles[], int particle); //mutation sequence (phase 3)
void nonElitist(int particles[], double fitness[]); //elitist reproduction
void elitist(int particles[], double fitness[]); //non elitist reproduction
double function(double x); //fitness function

void main()
{
    int particles[N]; //particle number array
    double position[N]; //position in X
    double fitness[N]; //fitness (Y = 1+sin(x) )
    int newParticles[N]; //particle of the next generation
    double random;
    double length = (b-a);
    srand((double)time(NULL));

    for (int i = 0; i<N; i++) //initiation of arrays
    {
        particles[i] = 0;
        position[i] = 0;
        fitness[i] = 0;
    }

    for (int i = 0; i<N; i++) //first generation, gives a random number to every particles and give them their fitness
    {
        random = ((double)rand()/(double)RAND_MAX);
        particles[i] = 1073741823*random;
        position[i] = (length*random)+a;
        fitness[i] = function(position[i]);
    }
    
    for (int i = 0; i<N; i++) //print the first generation
        printf("%f %f \n", position[i], fitness[i]);
    
    for (int h = 0; h<generation; h++) //do all the next generation until the end
    {
        if (doElitist == 0)
            nonElitist(particles, fitness);
        else
            elitist(particles, fitness);

        for (int i = 0; i<N; i++) //do the three step of reproduction
        {
            random = ((double)rand()/(double)RAND_MAX);
            int particle2 = ((double)rand()/(double)RAND_MAX)*99;
            
            if (random <= PR)
                recombination(random, particles, i, particle2);

            if (random <= PM)
                mutation(random, particles, i);

        }

        for (int i = 0; i<N; i++) //gives the new particules theirs positions and fitness for the next generation
        {
            position[i] = ((particles[i]/1073741823.0)*length)+a;
            fitness[i] = function(position[i]);
        }

        if (h+1 == 1 || h+1 == 10 || h+1 == 20 || h+1 == 30 || h+1 == 40 || h+1 == 50){//print for generation 10,20,30,40,50
            printf ("generation: %d\n", h+1);
            for (int i = 0; i<N; i++)
                printf ("%f %f \n", position[i], fitness[i]);
        }
    }
}

void elitist(int particles[], double fitness[]) //elitist function
{
    double maxFitness = 0;
    int bestParticlePos = 0;
    int bestParticle = 0;
    for (int i = 0; i<N; i++)// find the best particles
    {
        if (maxFitness < fitness[i])
        {
            bestParticlePos = i;
            maxFitness = fitness[i];
        }
    }
    bestParticle = particles[bestParticlePos];//save the best particule
    nonElitist(particles, fitness);//do a normal reproduction
    for (int i = 0; i<5; i++)//push the best particule 5 times in the array
    {
        particles[i] = bestParticle;
    }
}

void nonElitist(int particles[], double fitness[])// normal reproduction
{
    double minFitness = -0.1;
    double random;
    int newParticles[N];
    srand((double)time(NULL));
        for (int i = 0; i<N; i++) //avoid negative fitness issue (almost never used, can be removed)
        {
            if (minFitness > fitness[i])
                minFitness = fitness[i];
        }
        for (int i = 0; i<N; i++) //select a random particle and do a reproduction
        {
            random = ((double)rand()/(double)RAND_MAX);
            int select = reproduction(fitness, random, minFitness);
            newParticles[i] = particles[select];
        }
        for (int i = 0; i<N; i++)
        {
            particles[i] = newParticles[i];
        }
}

double function(double x) //the fitness function
{
    return (sin(x)+1);
}

void mutation(double random, int particles[], int particle) //the random mutation
{
    int bitMutation = 3000*random; //select a bit between the bit 0 and 30
    int *bin = intToBin(particles[particle]); //convert the particle to bit number and do the mutation (if 1 chang to 0, if 0 change to 1)
    if (bin[bitMutation] == 0) 
        bin[bitMutation] = 1;
    else
        bin[bitMutation] = 0;
    particles[particle] = binToInt(bin);
}

void recombination(double random, int particles[], int particle1, int particle2)// recombination for 2 particule
{
    int *bin1 = intToBin(particles[particle1]);//convert the two particles to binary numbers
    int *bin2 = intToBin2(particles[particle2]);
    int cutPos = ((random*100)-50)*0.6;// select where the cut will appens
    int tempbin1;
    int tempbin2;
    for (int i = 0; i < cutPos; i++) //do the cut and recombination
    {
        tempbin1 = bin1[i];
        tempbin2 = bin2[i];
        bin1[i] = tempbin2;
        bin2[i] = tempbin1;
    }
    particles[particle1] = binToInt(bin1);
    particles[particle2] = binToInt(bin2);
}

int *intToBin(int number) //convert int to binary string
{
    static int bin[30];
    int n = number;
    for (int i = 0; i < 30; i++) bin[i] = 0;
    for(int i = 0; n > 0; i++)    
    {    
        bin[i] = n%2;    
        n = n/2;    
    }
    return (bin);
}

int *intToBin2(int number)
{
    static int bin[30];
    int n = number;
    for (int i = 0; i < 30; i++) bin[i] = 0;
    for(int i = 0; n > 0; i++)    
    {    
        bin[i] = n%2;    
        n = n/2;    
    }
    return (bin);
}

int binToInt(int bits[]) //convert binary string to bit
{
    int a = 0;
    for (int i = 0; i<30; i++)
        if (bits[i] == 1) a += pow(2,i);
    return a;
}

int reproduction(double fitness[], double random, double minFitness) //reproduction phase
{
    double tempFitness[N];
    for (int i = 0; i < N; i++)
    {
        tempFitness[i] = fitness[i]-minFitness;
    }
    int sumOfFitness = 0;
    for (int i = 0; i < N; i++)
    {
        sumOfFitness += tempFitness[i];
    }
    random = random*sumOfFitness;
    for (int i = 0; i < N; i++)
    {
        if(random < tempFitness[i])
            return i;
        random -= tempFitness[i];
    }
}
