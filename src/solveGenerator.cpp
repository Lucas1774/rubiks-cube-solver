#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const int SCRAMBLE_LENGTH = 25;
const int NUMBER_OF_STICKERS = 24;
const int NUMBER_OF_EDGES = 12;
const int NUMBER_OF_CORNERS = 8;
const string PARITY_PATH = "src/parityAlgs.csv";
const string CORNERS_PATH = "src/cornerAlgs.csv";
const string EDGES_PATH = "src/edgeAlgs.csv";
const string SCRAMBLE_MOVES[6][3] = {{"U ", "U2 ", "U' "}, {"D ", "D2 ", "D' "}, {"F ", "F2 ", "F' "}, {"B ", "B2 ", "B' "}, {"R ", "R2 ", "R' "}, {"L ", "L2 ", "L' "}};
const int SOLVED_STATE_CORNERS[NUMBER_OF_STICKERS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
const int STICKER_PERMUTATIONS_CORNERS[6][12] = {{0, 3, 9, 6, 1, 4, 10, 7, 2, 5, 11, 8}, {12, 15, 18, 21, 13, 16, 19, 22, 14, 17, 20, 23}, {0, 17, 12, 5, 1, 15, 13, 3, 2, 16, 14, 4}, {6, 10, 21, 19, 7, 11, 22, 20, 8, 9, 23, 18}, {0, 7, 18, 16, 1, 8, 19, 17, 2, 6, 20, 15}, {3, 14, 21, 11, 4, 12, 22, 9, 5, 13, 23, 10}};
const string LETTERS_CORNERS = "0SFMXBRVPUNLITCOYJEKDAHZ";
const int ALPHABET_CORNERS[NUMBER_OF_STICKERS] = {23, 16, 5, 11, 22, 1, 15, 19, 14, 18, 12, 10, 8, 17, 2, 13, 20, 6, 4, 9, 3, 0, 7, 21};
const int SOLVED_STATE_EDGES[NUMBER_OF_STICKERS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
const int STICKER_PERMUTATIONS_EDGES[6][12] = {{0, 4, 6, 2, 1, 5, 7, 3}, {12, 14, 18, 16, 13, 15, 19, 17}, {0, 9, 12, 11, 1, 8, 13, 10}, {6, 23, 18, 21, 7, 22, 19, 20}, {2, 20, 14, 8, 3, 21, 15, 9}, {4, 10, 16, 22, 5, 11, 17, 23}};
const string LETTERS_EDGES = "0XRSMN\332UFJKB\315I\323O\301A\311EDPLZ";
const int ALPHABET_EDGES[NUMBER_OF_STICKERS] = {23, 22, 15, 16, 11, 12, 18, 19, 5, 6, 2, 1, 8, 20, 13, 9, 0, 17, 4, 7, 3, 14, 10, 21};

int main(int argc, char *argv[]) {
    // parsing letter pairs
    string buffer;
    string letter_pairs[529];
    if (argc > 1) {
        ifstream letter_pairs_file(argv[1]);
        for (int i = 0; i < 529; i++) {
            getline(letter_pairs_file, buffer, ',');
            letter_pairs[i] = buffer;
        }
    }

    // parsing algorithms
    string algorithms_parity[42];
    ifstream parity_file(PARITY_PATH);
    for (int i = 0; i < 42; i++) {
        getline(parity_file, buffer, ',');
        algorithms_parity[i] = buffer;
    }
    string algorithms_corners[441];
    ifstream corner_file(CORNERS_PATH);
    for (int i = 0; i < 441; i++) {
        getline(corner_file, buffer, ',');
        algorithms_corners[i] = buffer;
    }
    string algorithms_edges[484];
    ifstream edge_file(EDGES_PATH);
    for (int i = 0; i < 484; i++) {
        getline(edge_file, buffer, ',');
        algorithms_edges[i] = buffer;
    }

    setlocale(LC_ALL, "");
    srand(time(NULL));
    char option = 'y';
    while (option != 'n') {
        // generate scramble
        string scramble = "";
        vector<int> scramble_as_index_tuple_vector;
        int last_move_index = -1;
        int second_to_last_move_index = -1;
        int turn_layer, turn_iterator;
        for (int i = 0; i < SCRAMBLE_LENGTH; i++) {
            if (0 == i) {
                turn_layer = rand() % 6;
            } else if (1 == i || last_move_index / 2 != second_to_last_move_index / 2) {
                turn_layer = rand() % 5;
                if (turn_layer == last_move_index) {
                    turn_layer++;
                }
            } else {
                turn_layer = rand() % 4;
                if (turn_layer == last_move_index || turn_layer == second_to_last_move_index) {
                    if (0 == turn_layer % 2) {
                        turn_layer += 2;
                    } else {
                        turn_layer++;
                    }
                }
            }
            turn_iterator = rand() % 3;
            scramble += SCRAMBLE_MOVES[turn_layer][turn_iterator];
            scramble_as_index_tuple_vector.push_back(turn_layer);
            scramble_as_index_tuple_vector.push_back(turn_iterator);
            second_to_last_move_index = last_move_index;
            last_move_index = turn_layer;
        }

        // do scramble
        int scrambled_state_corners[NUMBER_OF_STICKERS], scrambled_state_edges[NUMBER_OF_STICKERS];
        copy(begin(SOLVED_STATE_CORNERS), end(SOLVED_STATE_CORNERS), scrambled_state_corners);
        copy(begin(SOLVED_STATE_EDGES), end(SOLVED_STATE_EDGES), scrambled_state_edges);
        for (int i = 0; i < scramble_as_index_tuple_vector.size() / 2; i++) {
            turn_layer = scramble_as_index_tuple_vector[i * 2];
            turn_iterator = scramble_as_index_tuple_vector[i * 2 + 1];
            for (int j = 0; j <= turn_iterator; j++) {
                int aux = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][0]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][0]] = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][3]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][3]] = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][2]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][2]] = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][1]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][1]] = aux;
                aux = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][4]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][4]] = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][7]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][7]] = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][6]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][6]] = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][5]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][5]] = aux;
                aux = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][8]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][8]] = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][11]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][11]] = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][10]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][10]] = scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][9]];
                scrambled_state_corners[STICKER_PERMUTATIONS_CORNERS[turn_layer][9]] = aux;

                aux = scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][0]];
                scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][0]] = scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][3]];
                scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][3]] = scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][2]];
                scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][2]] = scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][1]];
                scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][1]] = aux;
                aux = scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][4]];
                scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][4]] = scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][7]];
                scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][7]] = scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][6]];
                scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][6]] = scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][5]];
                scrambled_state_edges[STICKER_PERMUTATIONS_EDGES[turn_layer][5]] = aux;
            }
        }

        // generate corner targets vector
        vector<int> twist_corners;
        bool is_solved_corners[NUMBER_OF_CORNERS] = {false};
        int number_solved_corners = 0;
        for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
            // if its solved
            if (scrambled_state_corners[3 * i] == SOLVED_STATE_CORNERS[3 * i]) {
                is_solved_corners[i] = true;
                number_solved_corners++;
            }
            // if its passive
            else {
                if (scrambled_state_corners[3 * i] == SOLVED_STATE_CORNERS[3 * i + 1]) {
                    is_solved_corners[i] = true;
                    number_solved_corners++;
                    if (i != 0) {
                        twist_corners.push_back(scrambled_state_corners[3 * i + 1]);
                        twist_corners.push_back(scrambled_state_corners[3 * i + 2]);
                    }
                } else if (scrambled_state_corners[3 * i] == SOLVED_STATE_CORNERS[3 * i + 2]) {
                    is_solved_corners[i] = true;
                    number_solved_corners++;
                    if (i != 0) {
                        twist_corners.push_back(scrambled_state_corners[3 * i + 2]);
                        twist_corners.push_back(scrambled_state_corners[3 * i + 1]);
                    }
                }
            }
        }
        vector<int> path_corners;
        while (number_solved_corners < NUMBER_OF_CORNERS) {
            int new_target_corners;
            int cycle_piece_corners;
            for (int i = 0; i < NUMBER_OF_CORNERS; i++) {
                // pick buffer
                if (scrambled_state_corners[3 * i] > 2 && !is_solved_corners[i]) {
                    new_target_corners = scrambled_state_corners[3 * i];
                    cycle_piece_corners = i;
                    // if its not default buffer
                    if (cycle_piece_corners != 0) {
                        path_corners.push_back(3 * i);
                    }
                    is_solved_corners[cycle_piece_corners] = true;
                    number_solved_corners++;
                    break;
                }
            }
            // exhaust cycle
            while (new_target_corners / 3 != cycle_piece_corners) {
                path_corners.push_back(new_target_corners);
                is_solved_corners[new_target_corners / 3] = true;
                new_target_corners = scrambled_state_corners[new_target_corners];
                number_solved_corners++;
                // if its not default buffer
                if (new_target_corners / 3 == cycle_piece_corners && cycle_piece_corners != 0) {
                    path_corners.push_back(new_target_corners);
                }
            }
        }

        // generate edge targets vector
        vector<int> twist_edges;
        bool is_solved_edges[NUMBER_OF_EDGES] = {false};
        int number_solved_edges = 0;
        for (int i = 0; i < NUMBER_OF_EDGES; i++) {
            // if its solved
            if (scrambled_state_edges[2 * i] == SOLVED_STATE_EDGES[2 * i]) {
                is_solved_edges[i] = true;
                number_solved_edges++;
            }
            // if its passive
            else if (scrambled_state_edges[2 * i] == SOLVED_STATE_EDGES[2 * i + 1]) {
                is_solved_edges[i] = true;
                number_solved_edges++;
                if (i != 0) {
                    twist_edges.push_back(scrambled_state_edges[2 * i]);
                    twist_edges.push_back(scrambled_state_edges[2 * i + 1]);
                }
            }
        }
        vector<int> path_edges;
        while (number_solved_edges < NUMBER_OF_EDGES) {
            int new_target_edges;
            int cycle_piece_edges;
            for (int i = 0; i < NUMBER_OF_EDGES; i++) {
                // pick buffer
                if (scrambled_state_edges[2 * i] > 1 && !is_solved_edges[i]) {
                    new_target_edges = scrambled_state_edges[2 * i];
                    cycle_piece_edges = i;
                    // if its not default buffer
                    if (cycle_piece_edges != 0) {
                        path_edges.push_back(2 * i);
                    }
                    is_solved_edges[cycle_piece_edges] = true;
                    number_solved_edges++;
                    break;
                }
            }
            // exhaust cycle
            while (new_target_edges / 2 != cycle_piece_edges) {
                path_edges.push_back(new_target_edges);
                is_solved_edges[new_target_edges / 2] = true;
                new_target_edges = scrambled_state_edges[new_target_edges];
                number_solved_edges++;
                // if its not default buffer
                if (new_target_edges / 2 == cycle_piece_edges && cycle_piece_edges != 0) {
                    path_edges.push_back(new_target_edges);
                }
            }
        }

        // generate party
        vector<int> parity;
        if (path_corners.size() % 2 != 0) {
            parity.push_back(path_corners.back());
            path_corners.erase(path_corners.end() - 1);
            parity.push_back(path_edges.front());
            path_edges.erase(path_edges.begin());
        }

        // print
        cout << "\nscramble\n"
             << scramble;
        cout << "\n\ncorners\n";
        for (int i = 0; i < path_corners.size() / 2; i++) {
            cout << letter_pairs[(23 * ALPHABET_CORNERS[path_corners[2 * i]]) + (ALPHABET_CORNERS[path_corners[2 * i + 1]])];
            cout << " (" << LETTERS_CORNERS[path_corners[2 * i]] << "," << LETTERS_CORNERS[path_corners[2 * i + 1]] << ")"
                 << "\n";
            cout << algorithms_corners[((path_corners[2 * i] - 3) * 21) + (path_corners[2 * i + 1] - 3)] << "\n";
        }
        if (parity.size() > 0) {
            cout << "\nparity\n";
            cout << letter_pairs[(23 * ALPHABET_CORNERS[parity[0]]) + (ALPHABET_EDGES[parity[1]])];
            cout << " (" << LETTERS_CORNERS[parity[0]] << "," << LETTERS_EDGES[parity[1]] << ")"
                 << "\n";
            if (parity[1] == 3) {
                cout << algorithms_parity[21 + (parity[0] - 3)] << "\n";
            } else {
                cout << algorithms_parity[(parity[0] - 3)] << "\n";
            }
            if (parity[1] != 2 && parity[1] != 3) {
                cout << algorithms_edges[(parity[1] - 2)] << "\n";
            }
        }
        cout << "\nedges\n";
        for (int i = 0; i < path_edges.size() / 2; i++) {
            cout << letter_pairs[(23 * ALPHABET_EDGES[path_edges[2 * i]]) + (ALPHABET_EDGES[path_edges[2 * i + 1]])];
            cout << " (" << LETTERS_EDGES[path_edges[2 * i]] << "," << LETTERS_EDGES[path_edges[2 * i + 1]] << ")"
                 << "\n";
            cout << algorithms_edges[((path_edges[2 * i] - 2) * 22) + (path_edges[2 * i + 1] - 2)] << "\n";
        }
        if (twist_corners.size() > 0) {
            cout << "\ncorner twists\n";
            for (int i = 0; i < twist_corners.size() / 2; i++) {
                cout << letter_pairs[(23 * ALPHABET_CORNERS[twist_corners[2 * i]]) + (ALPHABET_CORNERS[twist_corners[2 * i + 1]])];
                cout << " (" << LETTERS_CORNERS[twist_corners[2 * i]] << "," << LETTERS_CORNERS[twist_corners[2 * i + 1]] << ")"
                     << "\n";
                cout << algorithms_corners[((twist_corners[2 * i] - 3) * 21) + (twist_corners[2 * i + 1] - 3)] << "\n";
            }
        }
        if (twist_edges.size() > 0) {
            cout << "\nedge flips\n";
            for (int i = 0; i < twist_edges.size() / 2; i++) {
                cout << letter_pairs[(23 * ALPHABET_EDGES[twist_edges[2 * i]]) + (ALPHABET_EDGES[twist_edges[2 * i + 1]])];
                cout << " (" << LETTERS_EDGES[twist_edges[2 * i]] << "," << LETTERS_EDGES[twist_edges[2 * i + 1]] << ")"
                     << "\n";
                cout << algorithms_edges[((twist_edges[2 * i] - 2) * 22) + (twist_edges[2 * i + 1] - 2)] << "\n";
            }
        }
        cout << "Again? [Y/n]\n";
        option = cin.get();
    }
    return 0;
}
