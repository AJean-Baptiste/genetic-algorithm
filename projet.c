#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double a = -4; //borne inférieur
double b = 4; //borne superieur
int N = 100; //nombre de particules par générations
int generation = 150; //nombre de génération
double PR = 0.5; //probabilité de recombination
double PM = 0.01; //probabilité de mutation
int doElitist = 0; //faire des génération elitistes(la meilleure particules est obligatoirement reproduite 5 fois) 1 = oui, 0 = non

int *intToBin(int number); //convertion d'un nombre en un string contenant sa valeur binaire
int *intToBin2(int number);
int binToInt(int bits[]); //convertion d'un string binaire en un nombre
int reproduction(double fitness[], double random, double minFitness); //séquence de reproduction (phase 1)
void recombination(double random, int particles[], int particle1, int particle2); //séquence de recombination (phase 2)
void mutation(double random, int particles[], int particle); //séquence de mutation (phase 3)
void nonElitist(int particles[], double fitness[]); //effectue une reproduction elitiste
void elitist(int particles[], double fitness[]); //effectue une reproduction non elitiste
double function(double x); //fonction qui definie le Fitness

void main()
{
    int particles[N];
    double position[N];
    double fitness[N];
    int newParticles[N];
    double random;
    double length = (b-a);
    srand((double)time(NULL));

    for (int i = 0; i<N; i++)
    {
        particles[i] = 0;
        position[i] = 0;
        fitness[i] = 0;
    }

    for (int i = 0; i<N; i++)
    {
        random = ((double)rand()/(double)RAND_MAX);
        particles[i] = 1073741823*random;
        position[i] = (length*random)+a;
        fitness[i] = function(position[i]);
    }
    
    for (int i = 0; i<N; i++)
        printf("%f %f \n", position[i], fitness[i]);
    
    for (int h = 0; h<generation; h++)
    {
        if (doElitist == 0)
            nonElitist(particles, fitness);
        else
            elitist(particles, fitness);

        for (int i = 0; i<N; i++)
        {
            random = ((double)rand()/(double)RAND_MAX);
            int particle2 = ((double)rand()/(double)RAND_MAX)*99;
            
            if (random <= PR)
                recombination(random, particles, i, particle2);

            if (random <= PM)
                mutation(random, particles, i);

        }

        for (int i = 0; i<N; i++)
        {
            position[i] = ((particles[i]/1073741823.0)*length)+a;
            fitness[i] = function(position[i]);
        }

        if (h+1 == 1 || h+1 == 10 || h+1 == 20 || h+1 == 30 || h+1 == 40 || h+1 == 50){
            printf ("generation: %d\n", h+1);
            for (int i = 0; i<N; i++)
                printf ("%f %f \n", position[i], fitness[i]);
        }
    }
}

void elitist(int particles[], double fitness[])
{
    double maxFitness = 0;
    int bestParticlePos = 0;
    int bestParticle = 0;
    for (int i = 0; i<N; i++)
    {
        if (maxFitness < fitness[i])
        {
            bestParticlePos = i;
            maxFitness = fitness[i];
        }
    }
    bestParticle = particles[bestParticlePos];
    nonElitist(particles, fitness);
    for (int i = 0; i<5; i++)
    {
        particles[i] = bestParticle;
    }
}

void nonElitist(int particles[], double fitness[])
{
    double minFitness = -0.1;
    double random;
    int newParticles[N];
    srand((double)time(NULL));
        for (int i = 0; i<N; i++)
        {
            if (minFitness > fitness[i])
                minFitness = fitness[i];
        }
        for (int i = 0; i<N; i++)
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

double function(double x)
{
    return (sin(x)+1);
}

void mutation(double random, int particles[], int particle)
{
    int bitMutation = 3000*random;
    int *bin = intToBin(particles[particle]);
    if (bin[bitMutation] == 0) 
        bin[bitMutation] = 1;
    else
        bin[bitMutation] = 0;
    particles[particle] = binToInt(bin);
}

void recombination(double random, int particles[], int particle1, int particle2)
{
    int *bin1 = intToBin(particles[particle1]);
    int *bin2 = intToBin2(particles[particle2]);
    int cutPos = ((random*100)-50)*0.6;
    int tempbin1;
    int tempbin2;
    for (int i = 0; i < cutPos; i++)
    {
        tempbin1 = bin1[i];
        tempbin2 = bin2[i];
        bin1[i] = tempbin2;
        bin2[i] = tempbin1;
    }
    particles[particle1] = binToInt(bin1);
    particles[particle2] = binToInt(bin2);
}

int *intToBin(int number)
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

int binToInt(int bits[])
{
    int a = 0;
    for (int i = 0; i<30; i++)
        if (bits[i] == 1) a += pow(2,i);
    return a;
}

int reproduction(double fitness[], double random, double minFitness)
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