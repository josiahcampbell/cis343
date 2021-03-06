#include <stdio.h>
#include "connect4_engine.h"
#include "CuTest.h"

#define EMPTY -1
#define NO_WINNER_YET -1

void ct_initialize(int num_rows, int num_cols, int array[num_rows][num_cols]) {
    int r, c;
    for (r = 0; r < num_rows; r++) {
        for (c = 0; c < num_cols; c++) {
            array[r][c] = EMPTY;
        }
    }
}

/*******************************************************************************************
 *
 * Test winner function
 *
 * Note: This is just a sample of how you can test your own winner
 * function.  You may need to modify this code depending on how your
 * organized your array.  I will not be testing your winner function
 * directly.
 *
 * Note:  Be sure to add tests for different directions, quadrants, and board sizes.
 *
 ******************************************************************************************/
void winner_horizontal_r0(CuTest *tc) {

    int num_rows = 7;
    int num_columns = 7;
    int array[num_rows][num_columns];
    int answer;
    ct_initialize(num_rows, num_columns, array);

    array[0][0] = 0;
    array[0][1] = 0;
    array[0][2] = 0;
    array[0][3] = 0;

    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "0s in bottom row", 0, answer);
}

void winner_diagonal_r48(CuTest *tc) {

    int num_rows = 100;
    int num_columns = 100;
    int array[num_rows][num_columns];
    int answer;
    ct_initialize(num_rows, num_columns, array);
    for (int i = 48; i < 55; ++i) {
        array[i][i + 2] = 0;
    }
    answer = winner(num_rows, num_columns, 7, array);
    CuAssertIntEquals_Msg(tc, "Centered diagonal win", 0, answer);
}

void loser_diagonal_r48(CuTest *tc) {
    /* Test counter to ensure that one "skip" in the win_count
     * doesn't let Player 1 win */
    int num_rows = 100;
    int num_columns = 100;
    int array[num_rows][num_columns];
    int answer;
    ct_initialize(num_rows, num_columns, array);
    for (int i = 48; i < 55; ++i) {
        array[i][i + 2] = 0;
    }
    // Win skip
    array[49][51] = 1;
    answer = winner(num_rows, num_columns, 7, array);
    CuAssertIntEquals_Msg(tc, "Centered diagonal loss", -1, answer);
}

/*******************************************************************************************
 *
 * Test place_token function
 *
 * Note: This is just a sample of how you can test your own
 * place_token function.  You may need to modify this code depending
 * on how your organized your array.  I will not be testing your
 * place_token function directly.
 *
 * Note:  Be sure to add tests for different directions, quadrants, and board sizes.
 *
 ******************************************************************************************/

void place_token_c1(CuTest *tc) {

    int num_rows = 7;
    int num_columns = 7;
    int array[num_rows][num_columns];
    ct_initialize(num_rows, num_columns, array);

    place_token(1, 3, num_rows, num_columns, array);

    // make sure there is a 1 at the bottom of column 3 and a -1 everywhere else
    CuAssertIntEquals_Msg(tc, "Drop 1 into empty column 3", 1, array[num_rows - 1][3]);
    int r, c;
    for (r = 0; r < num_rows; r++) {
        for (c = 0; c < num_columns; c++) {
            if (r != (num_rows - 1) || c != 3) {
                CuAssertIntEquals_Msg(tc, "Should be empty", EMPTY, array[r][c]);
            }
        }
    }
}

void place_token_c5(CuTest *tc) {

    int num_rows = 7;
    int num_columns = 7;
    int array[num_rows][num_columns];
    ct_initialize(num_rows, num_columns, array);

    place_token(1, 5, num_rows, num_columns, array);
    // make sure there is a 1 at the bottom of column 5 and a -1 everywhere else
    CuAssertIntEquals_Msg(tc, "Drop 1 into empty column 5", 1, array[num_rows - 1][5]);
    place_token(1, 5, num_rows, num_columns, array);
    CuAssertIntEquals_Msg(tc, "Drop 1 into column 5 on 1 full", 1, array[num_rows - 2][5]);
    place_token(1, 5, num_rows, num_columns, array);
    CuAssertIntEquals_Msg(tc, "Drop 1 into column 5 on 2 full", 1, array[num_rows - 3][5]);
    int r, c;
    for (r = 0; r < num_rows; r++) {
        for (c = 0; c < num_columns; c++) {
            if (r < (num_rows - 3) || c != 5) {
                CuAssertIntEquals_Msg(tc, "Should be empty", EMPTY, array[r][c]);
            }
        }
    }
}

void place_token_c99(CuTest *tc) {

    int num_rows = 100;
    int num_columns = 100;
    int array[num_rows][num_columns];
    ct_initialize(num_rows, num_columns, array);

    place_token(1, 99, num_rows, num_columns, array);
    CuAssertIntEquals_Msg(tc, "Drop 1 into empty column 99", 1, array[num_rows - 1][99]);
    int r, c;
    for (r = 0; r < num_rows; r++) {
        for (c = 0; c < num_columns; c++) {
            if (r != (num_rows - 1) || c != 99) {
                CuAssertIntEquals_Msg(tc, "Should be empty", EMPTY, array[r][c]);
            }
        }
    }
}

void place_token_invalid_index(CuTest *tc) {
    int num_rows = 100;
    int num_columns = 100;
    int array[num_rows][num_columns];
    int answer = 0;

    ct_initialize(num_rows, num_columns, array);
    answer = place_token(1, 100, num_rows, num_columns, array);
    CuAssertIntEquals_Msg(tc, "Should return error, invalid index ", -1, answer);
    answer = place_token(1, -1, num_rows, num_columns, array);
    CuAssertIntEquals_Msg(tc, "Should return error, invalid index ", -1, answer);
}

/*******************************************************************************************
 *
 * Test individual components of connect4_engine
 *
 ******************************************************************************************/

void check_full_board_pass(CuTest *tc) {
    int num_rows = 4;
    int num_columns = 4;
    int array[num_rows][num_columns];
    int answer = 0;
    ct_initialize(num_rows, num_columns, array);
    for (int row = num_rows - 1; row > -1; --row) {
        for (int col = 0; col < num_columns; ++col) {
            array[row][col] = 0; // A number which is not -1
        }
    }
    answer = check_full_board(num_rows, num_columns, array);
    CuAssertIntEquals_Msg(tc, "Board should be full", 4, answer);
}

void check_full_board_fail(CuTest *tc) {
    int num_rows = 7;
    int num_columns = 7;
    int array[num_rows][num_columns];
    int answer = 0;
    ct_initialize(num_rows, num_columns, array);
    // Fill all but the "top" row
    for (int row = num_rows - 1; row > 0; --row) {
        for (int col = 0; col < num_columns; ++col) {
            array[row][col] = 0; // A number which is not -1
        }
    }
    array[0][3] = 1; // Fill one cell in "top" row
    answer = check_full_board(num_rows, num_columns, array);
    CuAssertIntEquals_Msg(tc, "Only one cell should be filled in top row", 1, answer);
}

/*******************************************************************************************
 *
 * Test overall game play
 *
 * These tests below demonstrate how I will be testing your game.
 * These tests below should run wihtout modification.  If they don't
 * then something in your design conflicts with the tests and we need
 * to figure out what it is.  Make sure you find any such conflicts
 * and work with me to resole them well before the assignment due
 * date.
 *
 * Because they test the interaction between several methods, these
 * tests below are not technically "unit" tests.  If your tests for
 * your connect4_engine functions are thorough, you need not add many
 * tests to this section.
 *
 ******************************************************************************************/

void horizontal_row0(CuTest *tc) {
    int num_rows = 7;
    int num_columns = 7;
    int array[num_rows][num_columns];
    int answer;
    ct_initialize(num_rows, num_columns, array);

    place_token(0, 0, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Single 0 in column 0", NO_WINNER_YET, answer);

    place_token(0, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "0s in columns {0,1}", NO_WINNER_YET, answer);


    place_token(0, 2, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "0s in columns {0,1, 2}", NO_WINNER_YET, answer);


    place_token(0, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "4 in a row, horizontal", 0, answer);
}


void vertical_column1(CuTest *tc) {
    int num_rows = 7;
    int num_columns = 7;
    int array[num_rows][num_columns];
    int answer;
    ct_initialize(num_rows, num_columns, array);

    place_token(0, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Single 0 in column 1", NO_WINNER_YET, answer);

    place_token(0, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Two 0s in column 1", NO_WINNER_YET, answer);


    place_token(0, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Three 0s in column 1", NO_WINNER_YET, answer);


    place_token(0, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "4 in a row, vertical", 0, answer);
}

void forward_diagonal(CuTest *tc) {
    /*
       . . . . . . .
       . . . . . . .
       . . . . . . .
       . . . 0 . . .
       . . . 1 0 . .
       . . . 0 0 0 .
       1 . . 1 1 1 0
     */

    int num_rows = 7;
    int num_columns = 7;
    int array[num_rows][num_columns];
    int answer;
    ct_initialize(num_rows, num_columns, array);

    /* column 6*/
    place_token(0, 6, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 1", NO_WINNER_YET, answer);

    /* column 5*/
    place_token(1, 5, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 2", NO_WINNER_YET, answer);

    place_token(0, 5, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 3", NO_WINNER_YET, answer);


    /* column 4*/
    place_token(1, 4, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 4", NO_WINNER_YET, answer);

    place_token(0, 4, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 5", NO_WINNER_YET, answer);

    place_token(1, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 6", NO_WINNER_YET, answer);

    place_token(0, 4, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 7", NO_WINNER_YET, answer);

    /* column 3 */
    place_token(1, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 8", NO_WINNER_YET, answer);

    place_token(0, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 9", NO_WINNER_YET, answer);

    place_token(1, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 10", NO_WINNER_YET, answer);

    place_token(0, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 11 -- Winner!", 0, answer);
}


void backward_diagonal(CuTest *tc) {
    /*
       . . . . . . .
       . . . . . . .
       . . . . . . .
       . . . 0 . . .
       . . 0 1 . . .
       . 0 0 0 . . .
       0 1 1 1 . 1 .
     */

    int num_rows = 7;
    int num_columns = 7;
    int array[num_rows][num_columns];
    int answer;
    ct_initialize(num_rows, num_columns, array);

    /* column 0*/
    place_token(0, 0, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 1", NO_WINNER_YET, answer);

    /* column 1*/
    place_token(1, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 2", NO_WINNER_YET, answer);

    place_token(0, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 3", NO_WINNER_YET, answer);


    /* column 2*/
    place_token(1, 2, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 4", NO_WINNER_YET, answer);

    place_token(0, 2, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 5", NO_WINNER_YET, answer);

    place_token(1, 5, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 6", NO_WINNER_YET, answer);

    place_token(0, 2, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 7", NO_WINNER_YET, answer);

    /* column 3 */
    place_token(1, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 8", NO_WINNER_YET, answer);

    place_token(0, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 9", NO_WINNER_YET, answer);

    place_token(1, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 10", NO_WINNER_YET, answer);

    place_token(0, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 11 -- Winner!", 0, answer);
}


void tie_game(CuTest *tc) {
    /*
       1 0 1 1
       0 1 0 0
       1 0 0 0
       0 1 0 1
     */
// player then column
    int num_rows = 4;
    int num_columns = 4;
    int array[num_rows][num_columns];
    int answer;
    ct_initialize(num_rows, num_columns, array);

    /* column 0*/
    place_token(0, 0, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 1", NO_WINNER_YET, answer);

    place_token(1, 0, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 2", NO_WINNER_YET, answer);

    place_token(0, 0, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 3", NO_WINNER_YET, answer);

    place_token(1, 0, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 4", NO_WINNER_YET, answer);

    /* column 1*/
    place_token(1, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 5", NO_WINNER_YET, answer);

    place_token(0, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 6", NO_WINNER_YET, answer);

    place_token(1, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 7", NO_WINNER_YET, answer);

    place_token(0, 1, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 8", NO_WINNER_YET, answer);


    /* column 2*/
    place_token(0, 2, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 9", NO_WINNER_YET, answer);

    place_token(0, 2, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 10", NO_WINNER_YET, answer);

    place_token(0, 2, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 11", NO_WINNER_YET, answer);

    place_token(1, 2, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 12", NO_WINNER_YET, answer);

    /* column 3 */
    place_token(1, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 13", NO_WINNER_YET, answer);

    place_token(0, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 14", NO_WINNER_YET, answer);

    place_token(0, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 15", NO_WINNER_YET, answer);

    place_token(1, 3, num_rows, num_columns, array);
    answer = winner(num_rows, num_columns, 4, array);
    CuAssertIntEquals_Msg(tc, "Step 16 -- Winner!", 2, answer);
}

/*  setup */

CuSuite *c4_engine_suite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, winner_horizontal_r0);
    SUITE_ADD_TEST(suite, winner_diagonal_r48);
    SUITE_ADD_TEST(suite, loser_diagonal_r48);
    SUITE_ADD_TEST(suite, place_token_c1);
    SUITE_ADD_TEST(suite, place_token_c5);
    SUITE_ADD_TEST(suite, place_token_c99);
    SUITE_ADD_TEST(suite, place_token_invalid_index);
    SUITE_ADD_TEST(suite, check_full_board_pass);
    SUITE_ADD_TEST(suite, check_full_board_fail);
    SUITE_ADD_TEST(suite, horizontal_row0);
    SUITE_ADD_TEST(suite, vertical_column1);
    SUITE_ADD_TEST(suite, forward_diagonal);
    SUITE_ADD_TEST(suite, backward_diagonal);
    SUITE_ADD_TEST(suite, tie_game);
    return suite;
}
