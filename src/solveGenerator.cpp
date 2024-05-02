#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

#define scrambleLength 25
#define numberOfStickers 24
#define numberOfPieces_edges 12
#define numberOfPieces_corners 8

typedef int array24[numberOfStickers];
typedef int matrix6by8[6][8];
typedef int matrix6by12[6][12];
const string parity_path = "src/parityAlgs.csv";
const string corners_path = "src/cornerAlgs.csv";
const string edges_path = "src/edgeAlgs.csv";
const string emptyScramble = "";
const string scrambleMoves[6][3] = {{"U ", "U2 ", "U' "}, {"D ", "D2 ", "D' "}, {"F ", "F2 ", "F' "}, {"B ", "B2 ", "B' "}, {"R ", "R2 ", "R' "}, {"L ", "L2 ", "L' "}};
const array24 solvedState_corners = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
const matrix6by12 stickerPermutations_corners = {{0, 3, 9, 6, 1, 4, 10, 7, 2, 5, 11, 8}, {12, 15, 18, 21, 13, 16, 19, 22, 14, 17, 20, 23}, {0, 17, 12, 5, 1, 15, 13, 3, 2, 16, 14, 4}, {6, 10, 21, 19, 7, 11, 22, 20, 8, 9, 23, 18}, {0, 7, 18, 16, 1, 8, 19, 17, 2, 6, 20, 15}, {3, 14, 21, 11, 4, 12, 22, 9, 5, 13, 23, 10}};
const string letters_corners = "0SFMXBRVPUNLITCOYJEKDAHZ";
const array24 alphabet_corners = {23, 16, 5, 11, 22, 1, 15, 19, 14, 18, 12, 10, 8, 17, 2, 13, 20, 6, 4, 9, 3, 0, 7, 21};
const array24 solvedState_edges = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
const matrix6by8 stickerPermutations_edges{{0, 4, 6, 2, 1, 5, 7, 3}, {12, 14, 18, 16, 13, 15, 19, 17}, {0, 9, 12, 11, 1, 8, 13, 10}, {6, 23, 18, 21, 7, 22, 19, 20}, {2, 20, 14, 8, 3, 21, 15, 9}, {4, 10, 16, 22, 5, 11, 17, 23}};
const string letters_edges = "0XRSMN\332UFJKB\315I\323O\301A\311EDPLZ";
const array24 alphabet_edges = {23, 22, 15, 16, 11, 12, 18, 19, 5, 6, 2, 1, 8, 20, 13, 9, 0, 17, 4, 7, 3, 14, 10, 21};

int main(int argc, char *argv[])
{
    char option;
    string aux;
    int turnLayer, turnIterator;
    string scramble;
    vector<int> scrambleAsIndexTupleSequence;
    array24 scrambledState_corners, scrambledState_edges;
    int numberSolved_corners, newTarget_corners, cyclePiece_corners, aux_corners;
    bool isSolved_corners[numberOfPieces_corners];
    vector<int> path_corners;
    vector<int> twist_corners;
    int numberSolved_edges, newTarget_edges, cyclePiece_edges, aux_edges;
    bool isSolved_edges[numberOfPieces_edges];
    vector<int> path_edges;
    vector<int> twist_edges;
    vector<int> parity;
    bool hasParity, hasTwists_corners, hasTwists_edges, isDirect_parity;

    // parsing letter pairs
    string letterPairs[529];
    if (argc > 1)
    {
        ifstream letterPairsFile(argv[1]);
        for (int i = 0; i < 529; i++)
        {
            getline(letterPairsFile, aux, ',');
            letterPairs[i] = aux;
        }
    }

    // parsing algorithms
    string algorithms_parity[42];
    ifstream parityFile(parity_path);
    for (int i = 0; i < 42; i++)
    {
        getline(parityFile, aux, ',');
        algorithms_parity[i] = aux;
    }
    string algorithms_corners[441];
    ifstream cornerFile(corners_path);
    for (int i = 0; i < 441; i++)
    {
        getline(cornerFile, aux, ',');
        algorithms_corners[i] = aux;
    }
    string algorithms_edges[484];
    ifstream edgeFile(edges_path);
    for (int i = 0; i < 484; i++)
    {
        getline(edgeFile, aux, ',');
        algorithms_edges[i] = aux;
    }

    setlocale(LC_ALL, "");
    srand(time(NULL));
    while (option != 'n')
    {
        // reset
        scramble = emptyScramble;
        scrambleAsIndexTupleSequence.clear();
        for (int i = 0; i < numberOfStickers; i++)
        {
            scrambledState_corners[i] = solvedState_corners[i];
            scrambledState_edges[i] = solvedState_edges[i];
        }
        numberSolved_corners = 0;
        for (int i = 0; i < numberOfPieces_corners; i++)
        {
            isSolved_corners[i] = false;
        }
        path_corners.clear();
        twist_corners.clear();
        numberSolved_edges = 0;
        for (int i = 0; i < numberOfPieces_edges; i++)
        {
            isSolved_edges[i] = false;
        }
        path_edges.clear();
        twist_edges.clear();
        parity.clear();
        hasParity = false;
        hasTwists_corners = false;
        hasTwists_edges = false;
        isDirect_parity = false;

        // generate scramble
        int lastMoveIndex = -1;
        int secondToLastMoveIndex = -1;
        for (int i = 0; i < scrambleLength; i++)
        {
            if (0 == i)
            {
                turnLayer = rand() % 6;
            }
            else if (1 == i || lastMoveIndex / 2 != secondToLastMoveIndex / 2)
            {
                turnLayer = rand() % 5;
                if (turnLayer == lastMoveIndex)
                {
                    turnLayer++;
                }
            }
            else
            {
                turnLayer = rand() % 4;
                if (turnLayer == secondToLastMoveIndex)
                {
                    turnLayer += 2;
                }
                else if (turnLayer == lastMoveIndex)
                {
                    turnLayer++;
                }
            }
            turnIterator = rand() % 3;
            scramble += scrambleMoves[turnLayer][turnIterator];
            scrambleAsIndexTupleSequence.push_back(turnLayer);
            scrambleAsIndexTupleSequence.push_back(turnIterator);
            secondToLastMoveIndex = lastMoveIndex;
            lastMoveIndex = turnLayer;
        }

        // do scramble
        for (int i = 0; i < scrambleAsIndexTupleSequence.size() / 2; i++)
        {
            turnLayer = scrambleAsIndexTupleSequence[i * 2];
            turnIterator = scrambleAsIndexTupleSequence[i * 2 + 1];
            for (int j = 0; j <= turnIterator; j++)
            {
                int aux_corners, aux_edges;
                aux_corners = scrambledState_corners[stickerPermutations_corners[turnLayer][0]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][0]] = scrambledState_corners[stickerPermutations_corners[turnLayer][3]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][3]] = scrambledState_corners[stickerPermutations_corners[turnLayer][2]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][2]] = scrambledState_corners[stickerPermutations_corners[turnLayer][1]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][1]] = aux_corners;
                aux_corners = scrambledState_corners[stickerPermutations_corners[turnLayer][4]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][4]] = scrambledState_corners[stickerPermutations_corners[turnLayer][7]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][7]] = scrambledState_corners[stickerPermutations_corners[turnLayer][6]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][6]] = scrambledState_corners[stickerPermutations_corners[turnLayer][5]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][5]] = aux_corners;
                aux_corners = scrambledState_corners[stickerPermutations_corners[turnLayer][8]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][8]] = scrambledState_corners[stickerPermutations_corners[turnLayer][11]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][11]] = scrambledState_corners[stickerPermutations_corners[turnLayer][10]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][10]] = scrambledState_corners[stickerPermutations_corners[turnLayer][9]];
                scrambledState_corners[stickerPermutations_corners[turnLayer][9]] = aux_corners;

                aux_edges = scrambledState_edges[stickerPermutations_edges[turnLayer][0]];
                scrambledState_edges[stickerPermutations_edges[turnLayer][0]] = scrambledState_edges[stickerPermutations_edges[turnLayer][3]];
                scrambledState_edges[stickerPermutations_edges[turnLayer][3]] = scrambledState_edges[stickerPermutations_edges[turnLayer][2]];
                scrambledState_edges[stickerPermutations_edges[turnLayer][2]] = scrambledState_edges[stickerPermutations_edges[turnLayer][1]];
                scrambledState_edges[stickerPermutations_edges[turnLayer][1]] = aux_edges;
                aux_edges = scrambledState_edges[stickerPermutations_edges[turnLayer][4]];
                scrambledState_edges[stickerPermutations_edges[turnLayer][4]] = scrambledState_edges[stickerPermutations_edges[turnLayer][7]];
                scrambledState_edges[stickerPermutations_edges[turnLayer][7]] = scrambledState_edges[stickerPermutations_edges[turnLayer][6]];
                scrambledState_edges[stickerPermutations_edges[turnLayer][6]] = scrambledState_edges[stickerPermutations_edges[turnLayer][5]];
                scrambledState_edges[stickerPermutations_edges[turnLayer][5]] = aux_edges;
            }
        }

        // generate corner targets vector
        for (int i = 0; i < numberOfPieces_corners; i++)
        {
            // if its solved
            if (scrambledState_corners[3 * i] == solvedState_corners[3 * i])
            {
                isSolved_corners[i] = true;
                numberSolved_corners++;
            }
            // if its passive
            else
            {
                if (scrambledState_corners[3 * i] == solvedState_corners[3 * i + 1])
                {
                    isSolved_corners[i] = true;
                    numberSolved_corners++;
                    if (i != 0)
                    {
                        twist_corners.push_back(scrambledState_corners[3 * i + 1]);
                        twist_corners.push_back(scrambledState_corners[3 * i + 2]);
                        hasTwists_corners = true;
                    }
                }
                else if (scrambledState_corners[3 * i] == solvedState_corners[3 * i + 2])
                {
                    isSolved_corners[i] = true;
                    numberSolved_corners++;
                    if (i != 0)
                    {
                        twist_corners.push_back(scrambledState_corners[3 * i + 2]);
                        twist_corners.push_back(scrambledState_corners[3 * i + 1]);
                        hasTwists_corners = true;
                    }
                }
            }
        }
        while (numberSolved_corners < numberOfPieces_corners)
        {
            for (int i = 0; i < numberOfPieces_corners; i++)
            {
                // pick buffer
                if (scrambledState_corners[3 * i] != 0 && scrambledState_corners[3 * i] != 1 && scrambledState_corners[3 * i] != 2 && !isSolved_corners[i])
                {
                    newTarget_corners = scrambledState_corners[3 * i];
                    cyclePiece_corners = i;
                    // if its not default buffer
                    if (cyclePiece_corners != 0)
                    {
                        path_corners.push_back(3 * i);
                    }
                    isSolved_corners[cyclePiece_corners] = true;
                    numberSolved_corners++;
                    break;
                }
            }
            // exhaust cycle
            while (floor(newTarget_corners / 3) != cyclePiece_corners)
            {
                path_corners.push_back(newTarget_corners);
                aux_corners = floor(newTarget_corners / 3);
                isSolved_corners[aux_corners] = true;
                newTarget_corners = scrambledState_corners[newTarget_corners];
                numberSolved_corners++;
                // if its not default buffer
                if (floor(newTarget_corners / 3) == cyclePiece_corners && cyclePiece_corners != 0)
                {
                    path_corners.push_back(newTarget_corners);
                }
            }
        }
        // generate edge targets vector
        for (int i = 0; i < numberOfPieces_edges; i++)
        {
            // if its solved
            if (scrambledState_edges[2 * i] == solvedState_edges[2 * i])
            {
                isSolved_edges[i] = true;
                numberSolved_edges++;
            }
            // if its passive
            else if (scrambledState_edges[2 * i] == solvedState_edges[2 * i + 1])
            {
                isSolved_edges[i] = true;
                numberSolved_edges++;
                if (i != 0)
                {
                    twist_edges.push_back(scrambledState_edges[2 * i]);
                    twist_edges.push_back(scrambledState_edges[2 * i + 1]);
                    hasTwists_edges = true;
                }
            }
        }
        while (numberSolved_edges < numberOfPieces_edges)
        {
            for (int i = 0; i < numberOfPieces_edges; i++)
            {
                // pick buffer
                if (scrambledState_edges[2 * i] != 0 && scrambledState_edges[2 * i] != 1 && !isSolved_edges[i])
                {
                    newTarget_edges = scrambledState_edges[2 * i];
                    cyclePiece_edges = i;
                    // if its not default buffer
                    if (cyclePiece_edges != 0)
                    {
                        path_edges.push_back(2 * i);
                    }
                    isSolved_edges[cyclePiece_edges] = true;
                    numberSolved_edges++;
                    break;
                }
            }
            // exhaust cycle
            while (floor(newTarget_edges / 2) != cyclePiece_edges)
            {
                path_edges.push_back(newTarget_edges);
                aux_edges = floor(newTarget_edges / 2);
                isSolved_edges[aux_edges] = true;
                newTarget_edges = scrambledState_edges[newTarget_edges];
                numberSolved_edges++;
                // if its not default buffer
                if (floor(newTarget_edges / 2) == cyclePiece_edges && cyclePiece_edges != 0)
                {
                    path_edges.push_back(newTarget_edges);
                }
            }
        }

        // generate party
        if (path_corners.size() % 2 != 0)
        {
            hasParity = true;
            parity.push_back(path_corners.back());
            path_corners.erase(path_corners.end() - 1);
            parity.push_back(path_edges.front());
            path_edges.erase(path_edges.begin());
        }

        // print
        cout << "\nscramble\n"
             << scramble;
        cout << "\n\ncorners\n";
        for (int i = 0; i < path_corners.size() / 2; i++)
        {
            cout << letterPairs[(23 * alphabet_corners[path_corners[2 * i]]) + (alphabet_corners[path_corners[2 * i + 1]])];
            cout << " (" << letters_corners[path_corners[2 * i]] << "," << letters_corners[path_corners[2 * i + 1]] << ")"
                 << "\n";
            cout << algorithms_corners[((path_corners[2 * i] - 3) * 21) + (path_corners[2 * i + 1] - 3)] << "\n";
        }
        if (hasParity)
        {
            if (parity[1] == 2 || parity[1] == 3)
            {
                isDirect_parity = true;
            }
            cout << "\nparity\n";
            cout << letterPairs[(23 * alphabet_corners[parity[0]]) + (alphabet_edges[parity[1]])];
            cout << " (" << letters_corners[parity[0]] << "," << letters_edges[parity[1]] << ")"
                 << "\n";
            if (parity[1] == 3)
            {
                cout << algorithms_parity[21 + (parity[0] - 3)] << "\n";
            }
            else
            {
                cout << algorithms_parity[(parity[0] - 3)] << "\n";
            }
            if (!isDirect_parity)
            {
                cout << algorithms_edges[(parity[1] - 2)] << "\n";
            }
        }
        cout << "\nedges\n";
        for (int i = 0; i < path_edges.size() / 2; i++)
        {
            cout << letterPairs[(23 * alphabet_edges[path_edges[2 * i]]) + (alphabet_edges[path_edges[2 * i + 1]])];
            cout << " (" << letters_edges[path_edges[2 * i]] << "," << letters_edges[path_edges[2 * i + 1]] << ")"
                 << "\n";
            cout << algorithms_edges[((path_edges[2 * i] - 2) * 22) + (path_edges[2 * i + 1] - 2)] << "\n";
        }
        if (hasTwists_corners)
        {
            cout << "\ncorner twists\n";
            for (int i = 0; i < twist_corners.size() / 2; i++)
            {
                cout << letterPairs[(23 * alphabet_corners[twist_corners[2 * i]]) + (alphabet_corners[twist_corners[2 * i + 1]])];
                cout << " (" << letters_corners[twist_corners[2 * i]] << "," << letters_corners[twist_corners[2 * i + 1]] << ")"
                     << "\n";
                cout << algorithms_corners[((twist_corners[2 * i] - 3) * 21) + (twist_corners[2 * i + 1] - 3)] << "\n";
            }
        }
        if (hasTwists_edges)
        {
            cout << "\nedge flips\n";
            for (int i = 0; i < twist_edges.size() / 2; i++)
            {
                cout << letterPairs[(23 * alphabet_edges[twist_edges[2 * i]]) + (alphabet_edges[twist_edges[2 * i + 1]])];
                cout << " (" << letters_edges[twist_edges[2 * i]] << "," << letters_edges[twist_edges[2 * i + 1]] << ")"
                     << "\n";
                cout << algorithms_edges[((twist_edges[2 * i] - 2) * 22) + (twist_edges[2 * i + 1] - 2)] << "\n";
            }
        }
        cout << "Again? [Y/n]\n";
        option = cin.get();
    }
    return 0;
}