#genetic-algorithm

this program uses evolution to solve a problem faster, in that case it uses genetic.

the problem to solve here is to find a maximum in a function, here it's the sinus+1 function between -4 and 4.
to do that, we will create 100 cell that will have their own DNA represented as a series of 30 bits, those 30 bits represent a value between -4 and 4.
each cell will have a score using a fitness value, the higher the better it is. this fitness will be the sinus+1 function.

Each step in the program, 50 of the lowest fitness cells will be selected and killed, the others cells will be crossed and some mutation will occurs randomly on their DNA.

crossing example:
cell 44: 010110010|011100111100100011011
cell 87: 101101111|000101101000110101101
                 cut
after crossing:
cell 44: 010110010|000101101000110101101
cell 87: 101101111|011100111100(0)00011011
                          random mutation
                          
after that, we once again have 100 cell, and we reitterate that for each generation until the problem is solved:

Example:
![Animation](https://user-images.githubusercontent.com/110237135/182137869-aa2f63c1-5a64-446f-9323-30baa43c612a.gif)
