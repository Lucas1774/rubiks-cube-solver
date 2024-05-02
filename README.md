# Program

I wrote this around 6 months after my first ever hello world program, so I would change a lot today. It was also my very first contact with c++. I didn't know how to compile c++ code, in fact, I must have heard the word "compile" a couple of times before.
I added the parsing functions just now, since that sounded like too much for me to do back then, as well as the logic of the arguments (which I'm glad I could fit in with no work at all).
The program is definitely a little over the place and screams for some structuring, but it works like a clock. You can check it out [here](https://alg.cubing.net/).

It generates a Rubik's cube scramble and then solves it using an algorithm database and the "3-style" method, which is usually used by humans to solve it blindfolded. These algorithms are the ones I use, so the program pretty much solves it as I would. It could be possible to obtain these algorithms or the whole solution using pathfinding, but there is plenty of software that already does this. This is supposed to be a somewhat (not really) customizable humanlike solve generator.

To generate the scramble it uses two randomly generated seeds, one linked to the layer to turn, the other to the turn to make (CW, CCW or double). The scrambled state is obtained during the scramble, which is definitely an ugly solution.
Then, in a VERY oversimplified way, it observes the path that the pieces should follow for them to end up in their correct spot, and creates a vector of "targets". These target vectors are then interpreted to display their associated algorithm sequence.

Unfortunately, most of its customization is only possible within the code: the buffer choice, the letter key, the order in which the three tasks are done or the orientation method (and others I might be forgetting). There is too much style variety for me to make a "universal 3-style Rubik's cube solver". I <s>am</s> was a junior computer science student, after all, driven by the shame of not knowing a single programming language after a whole semester.

## Usage

It can be run without arguments or with one that should be the path of a one-line (I used to compile this data, that's why it's stored in a single line instead of in a table) CSV file with the memorization key. This key is based on the letter pair memorization technique, used also for memorizing cards. I won't share mine for reasons that you will find very reasonable if you think about it.  
The other .csv files must be inside a "src" folder in the same directory as the executable.

## Key

The algorithms in the .csv files should follow the order you would expect, excluding the buffer pieces (UFR and UF), eg, the first algorithm in the corners file would be 3-3, this is, move the sticker 3 to spot 3, which obviously doesn't require any algorithm at all (but does require an entrance in the .csv file -> ",,"). The second one would be 3-4, this is, rotating the UFL corner clockwise. The fourth algorithm would be 3-6, which is the one associated with the UFR-UFL-UBR 3-cycle.  
For the parity algorithms, the first two are (UF-UR)-(UFR-UFL) and (UF-UR)-(UFR-FUL), so first every corner and the UR edge sticker, then every corner and the RU sticker.  
For the letter pairs, every combination but those including buffer stickers (23 * 23) must appear in the order ABCDEFJHIKLMNOPRSTUVYZX (it's not exactly alphabetic because I actually use numbers for the *-X or X-* tuples), this is, A-A, A-B... ...X-X.  
If I were to rewrite this program today, I would make it so, for sequences, every combination starting with 0-0 and ending with 23-23 must appear (even for parity), and for letter pairs, it appears every one starting with whatever letter the sticker 0 corresponds to and ending with whichever one the sticker 23 does. I could even store this data in a mySQL database, although there wouldn't be much point. In any case, it would be stored in tables and the access would be direct and not the indexing nightmare this was.

This is the key the program uses to define scrambled states and pick algorithms from the database.
The letter code is implemented in the "alphabet" arrays: the value in spot 1 of the corners one is 16 because sticker 1 corresponds to the letter 16 (S).

                         |-----||-----||-----|
                         |  22 ||  19 ||  20 |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  22 ||     ||  20 |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  11 ||  7  ||  7  |
                         |-----||-----||-----|
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
    |  23 ||  23 ||  10 ||  9  ||  6  ||  6  ||  8  ||  21 ||  19 |
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
    |  17 ||     ||  5  ||  4  ||     ||  2  ||  3  ||     ||  15 |
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
    |  13 ||  11 ||  5  ||  3  ||  0  ||  0  ||  1  ||  9  ||  17 |
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
                         |-----||-----||-----|
                         |  4  ||  1  ||  2  |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  10 ||     ||  8  |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  14 ||  13 ||  16 |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  12 ||  12 ||  15 |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  16 ||     ||  14 |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  21 ||  18 ||  18 |
                         |-----||-----||-----|
    ________________________________________________________________

                         |-----||-----||-----|
                         |  H  ||  H  ||  D  |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  L  ||     ||  D  |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  L  ||  V  ||  V  |
                         |-----||-----||-----|
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
    |  Z  ||  Z  ||  N  ||  U  ||  U  ||  R  ||  P  ||  P  ||  K  |
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
    |  T  ||     ||  N  ||  M  ||     ||  R  ||  S  ||     ||  K  |
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
    |  T  ||  B  ||  B  ||  M  ||  0  ||  0  ||  S  ||  J  ||  J  |
    |-----||-----||-----||-----||-----||-----||-----||-----||-----|
                         |-----||-----||-----|
                         |  X  ||  X  ||  F  |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  C  ||     ||  F  |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  C  ||  Y  ||  Y  |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  I  ||  I  ||  O  |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  A  ||     ||  O  |
                         |-----||-----||-----|
                         |-----||-----||-----|
                         |  A  ||  E  ||  E  |
                         |-----||-----||-----|
                         