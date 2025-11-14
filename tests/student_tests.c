#include "unit_tests.h"

TestSuite(student_tests, .timeout=TEST_TIMEOUT); 

/* Test add_mats_sf - Matrix Addition */
Test(student_tests, test_add_simple) {
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    matrix_sf *B = copy_matrix(2, 2, (int[]){5, 6, 7, 8});
    matrix_sf *C = add_mats_sf(A, B);
    expect_matrices_equal(C, 2, 2, (int[]){6, 8, 10, 12});
    cr_expect_eq(C->num_rows, 2);
    cr_expect_eq(C->num_cols, 2);
    free(A);
    free(B);
    free(C);
}

Test(student_tests, test_add_with_negatives) {
    matrix_sf *A = copy_matrix(2, 2, (int[]){-1, -2, 3, -4});
    matrix_sf *B = copy_matrix(2, 2, (int[]){5, -6, -7, 8});
    matrix_sf *C = add_mats_sf(A, B);
    expect_matrices_equal(C, 2, 2, (int[]){4, -8, -4, 4});
    free(A);
    free(B);
    free(C);
}

/* Test mult_mats_sf - Matrix Multiplication */
Test(student_tests, test_mult_simple) {
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *B = copy_matrix(3, 2, (int[]){7, 8, 9, 10, 11, 12});
    matrix_sf *C = mult_mats_sf(A, B);
    // Result: [1*7+2*9+3*11, 1*8+2*10+3*12] = [58, 64]
    //         [4*7+5*9+6*11, 4*8+5*10+6*12] = [139, 154]
    expect_matrices_equal(C, 2, 2, (int[]){58, 64, 139, 154});
    free(A);
    free(B);
    free(C);
}

Test(student_tests, test_mult_1x1) {
    matrix_sf *A = copy_matrix(1, 1, (int[]){5});
    matrix_sf *B = copy_matrix(1, 1, (int[]){7});
    matrix_sf *C = mult_mats_sf(A, B);
    expect_matrices_equal(C, 1, 1, (int[]){35});
    free(A);
    free(B);
    free(C);
}

/* Test transpose_mat_sf - Matrix Transpose */
Test(student_tests, test_transpose_simple) {
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *B = transpose_mat_sf(A);
    expect_matrices_equal(B, 3, 2, (int[]){1, 4, 2, 5, 3, 6});
    cr_expect_eq(B->num_rows, 3);
    cr_expect_eq(B->num_cols, 2);
    free(A);
    free(B);
}

Test(student_tests, test_transpose_square) {
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    matrix_sf *B = transpose_mat_sf(A);
    expect_matrices_equal(B, 2, 2, (int[]){1, 3, 2, 4});
    free(A);
    free(B);
}

/* Test create_matrix_sf - Matrix Creation from String */
Test(student_tests, test_create_matrix_simple) {
    matrix_sf *mat = create_matrix_sf('T', "2 2 [1 2; 3 4;]");
    cr_expect_not_null(mat);
    cr_expect_eq(mat->name, 'T');
    cr_expect_eq(mat->num_rows, 2);
    cr_expect_eq(mat->num_cols, 2);
    expect_matrices_equal(mat, 2, 2, (int[]){1, 2, 3, 4});
    free(mat);
}

Test(student_tests, test_create_matrix_with_spaces) {
    matrix_sf *mat = create_matrix_sf('X', "  3  2  [  10  20  ;  30  40  ;  50  60  ;  ]  ");
    cr_expect_not_null(mat);
    cr_expect_eq(mat->name, 'X');
    cr_expect_eq(mat->num_rows, 3);
    cr_expect_eq(mat->num_cols, 2);
    expect_matrices_equal(mat, 3, 2, (int[]){10, 20, 30, 40, 50, 60});
    free(mat);
}

Test(student_tests, test_create_matrix_negatives) {
    matrix_sf *mat = create_matrix_sf('N', "2 2 [-1 -2; -3 -4;]");
    cr_expect_not_null(mat);
    expect_matrices_equal(mat, 2, 2, (int[]){-1, -2, -3, -4});
    free(mat);
}

/* Test insert_bst_sf - BST Insertion */
Test(student_tests, test_insert_bst_empty) {
    bst_sf *root = NULL;
    matrix_sf *A = malloc(sizeof(matrix_sf));
    A->name = 'A';
    root = insert_bst_sf(A, root);
    cr_expect_not_null(root);
    cr_expect_eq(root->mat->name, 'A');
    cr_expect_null(root->left_child);
    cr_expect_null(root->right_child);
    free(A);
    free(root);
}

Test(student_tests, test_insert_bst_multiple) {
    bst_sf *root = NULL;
    char names[] = "DBAEC";
    matrix_sf *mats[5];
    for (int i = 0; i < 5; i++) {
        mats[i] = malloc(sizeof(matrix_sf));
        mats[i]->name = names[i];
        root = insert_bst_sf(mats[i], root);
    }
    // Verify BST structure (inorder should be sorted: ABCDE)
    cr_expect_not_null(root);
    // Check that we can find all inserted matrices
    for (int i = 0; i < 5; i++) {
        matrix_sf *found = find_bst_sf(names[i], root);
        cr_expect_not_null(found);
        cr_expect_eq(found->name, names[i]);
    }
    // Clean up
    for (int i = 0; i < 5; i++) {
        free(mats[i]);
    }
    free_bst_sf(root);
}

/* Test find_bst_sf - BST Search */
Test(student_tests, test_find_bst_exists) {
    bst_sf *root = NULL;
    matrix_sf *A = malloc(sizeof(matrix_sf));
    A->name = 'A';
    matrix_sf *B = malloc(sizeof(matrix_sf));
    B->name = 'B';
    matrix_sf *C = malloc(sizeof(matrix_sf));
    C->name = 'C';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    root = insert_bst_sf(C, root);
    
    matrix_sf *found = find_bst_sf('B', root);
    cr_expect_not_null(found);
    cr_expect_eq(found->name, 'B');
    
    free(A);
    free(B);
    free(C);
    free_bst_sf(root);
}

Test(student_tests, test_find_bst_not_exists) {
    bst_sf *root = NULL;
    matrix_sf *A = malloc(sizeof(matrix_sf));
    A->name = 'A';
    root = insert_bst_sf(A, root);
    
    matrix_sf *found = find_bst_sf('Z', root);
    cr_expect_null(found);
    
    free(A);
    free_bst_sf(root);
}

Test(student_tests, test_find_bst_empty) {
    matrix_sf *found = find_bst_sf('A', NULL);
    cr_expect_null(found);
}

/* Test free_bst_sf - BST Deallocation */
Test(student_tests, test_free_bst_empty) {
    free_bst_sf(NULL);  // Should not crash
    cr_expect(1);  // If we get here, it worked
}

Test(student_tests, test_free_bst_single) {
    bst_sf *root = NULL;
    matrix_sf *A = malloc(sizeof(matrix_sf));
    A->name = 'A';
    root = insert_bst_sf(A, root);
    free_bst_sf(root);  // Should free both node and matrix
    cr_expect(1);  // If we get here, no crash
}

/* Test infix2postfix_sf - Infix to Postfix Conversion */
Test(student_tests, test_infix2postfix_simple) {
    char *result = infix2postfix_sf("A+B");
    cr_expect_not_null(result);
    cr_expect_str_eq(result, "AB+");
    free(result);
}

Test(student_tests, test_infix2postfix_precedence) {
    char *result = infix2postfix_sf("A+B*C");
    cr_expect_not_null(result);
    cr_expect_str_eq(result, "ABC*+");
    free(result);
}

Test(student_tests, test_infix2postfix_parentheses) {
    char *result = infix2postfix_sf("(A+B)*(C+D)");
    cr_expect_not_null(result);
    cr_expect_str_eq(result, "AB+CD+*");
    free(result);
}

Test(student_tests, test_infix2postfix_transpose) {
    char *result = infix2postfix_sf("A'");
    cr_expect_not_null(result);
    cr_expect_str_eq(result, "A'");
    free(result);
}

Test(student_tests, test_infix2postfix_complex) {
    char *result = infix2postfix_sf("A'*B+C");
    cr_expect_not_null(result);
    cr_expect_str_eq(result, "A'B*C+");
    free(result);
}

Test(student_tests, test_infix2postfix_with_spaces) {
    char *result = infix2postfix_sf("A + B * C");
    cr_expect_not_null(result);
    cr_expect_str_eq(result, "ABC*+");
    free(result);
}

/* Test evaluate_expr_sf - Expression Evaluation */
Test(student_tests, test_evaluate_simple_add) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    A->name = 'A';
    matrix_sf *B = copy_matrix(2, 2, (int[]){5, 6, 7, 8});
    B->name = 'B';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    
    matrix_sf *result = evaluate_expr_sf('C', "A+B", root);
    cr_expect_not_null(result);
    cr_expect_eq(result->name, 'C');
    expect_matrices_equal(result, 2, 2, (int[]){6, 8, 10, 12});
    
    free(result);
    free_bst_sf(root);
}

Test(student_tests, test_evaluate_mult) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    A->name = 'A';
    matrix_sf *B = copy_matrix(2, 2, (int[]){2, 0, 1, 2});
    B->name = 'B';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    
    matrix_sf *result = evaluate_expr_sf('C', "A*B", root);
    cr_expect_not_null(result);
    // [1*2+2*1, 1*0+2*2] = [4, 4]
    // [3*2+4*1, 3*0+4*2] = [10, 8]
    expect_matrices_equal(result, 2, 2, (int[]){4, 4, 10, 8});
    
    free(result);
    free_bst_sf(root);
}

Test(student_tests, test_evaluate_transpose) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    A->name = 'A';
    root = insert_bst_sf(A, root);
    
    matrix_sf *result = evaluate_expr_sf('B', "A'", root);
    cr_expect_not_null(result);
    cr_expect_eq(result->name, 'B');
    expect_matrices_equal(result, 3, 2, (int[]){1, 4, 2, 5, 3, 6});
    
    free(result);
    free_bst_sf(root);
}

Test(student_tests, test_evaluate_complex) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 0, 0, 1});
    A->name = 'A';
    matrix_sf *B = copy_matrix(2, 2, (int[]){2, 0, 0, 2});
    B->name = 'B';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    
    matrix_sf *result = evaluate_expr_sf('C', "A+B*B", root);
    cr_expect_not_null(result);
    // B*B = [4,0,0,4], then A+B*B = [5,0,0,5]
    expect_matrices_equal(result, 2, 2, (int[]){5, 0, 0, 5});
    
    free(result);
    free_bst_sf(root);
}

/* Test execute_script_sf - Script Execution */
Test(student_tests, test_execute_script_simple) {
    // Create a temporary script file
    FILE *f = fopen("test_script_temp.txt", "w");
    fprintf(f, "A = 2 2 [1 2; 3 4;]\n");
    fprintf(f, "B = 2 2 [5 6; 7 8;]\n");
    fprintf(f, "C = A + B\n");
    fclose(f);
    
    matrix_sf *result = execute_script_sf("test_script_temp.txt");
    cr_expect_not_null(result);
    cr_expect_eq(result->name, 'C');
    expect_matrices_equal(result, 2, 2, (int[]){6, 8, 10, 12});
    
    // Clean up - we need to free the BST that execute_script_sf created
    // But we don't have access to it, so we'll just free the result
    // In a real scenario, execute_script_sf would need to return the BST or
    // we'd need another way to clean it up. For now, we'll just free result.
    free(result);
    
    // Remove temp file
    remove("test_script_temp.txt");
}

Test(student_tests, test_execute_script_with_mult) {
    FILE *f = fopen("test_script_temp2.txt", "w");
    fprintf(f, "A = 2 2 [1 2; 3 4;]\n");
    fprintf(f, "B = 2 2 [2 0; 1 2;]\n");
    fprintf(f, "C = A * B\n");
    fclose(f);
    
    matrix_sf *result = execute_script_sf("test_script_temp2.txt");
    cr_expect_not_null(result);
    cr_expect_eq(result->name, 'C');
    expect_matrices_equal(result, 2, 2, (int[]){4, 4, 10, 8});
    
    free(result);
    remove("test_script_temp2.txt");
}

/* Test cases from homework examples */
Test(student_tests, test_case1_basic_add) {
    matrix_sf *A = copy_matrix(1, 1, (int[]){5});
    matrix_sf *B = copy_matrix(1, 1, (int[]){3});
    matrix_sf *C = add_mats_sf(A, B);
    expect_matrices_equal(C, 1, 1, (int[]){8});
    free(A);
    free(B);
    free(C);
}

Test(student_tests, test_case2_transpose) {
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    matrix_sf *B = transpose_mat_sf(A);
    expect_matrices_equal(B, 3, 2, (int[]){1, 4, 2, 5, 3, 6});
    free(A);
    free(B);
}

Test(student_tests, test_case3_mult) {
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    matrix_sf *B = copy_matrix(2, 1, (int[]){5, 6});
    matrix_sf *C = mult_mats_sf(A, B);
    expect_matrices_equal(C, 2, 1, (int[]){17, 39});
    free(A);
    free(B);
    free(C);
}

Test(student_tests, test_case4_complex_parentheses) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    A->name = 'A';
    matrix_sf *B = copy_matrix(2, 2, (int[]){2, 0, 1, 2});
    B->name = 'B';
    matrix_sf *C = copy_matrix(2, 2, (int[]){1, 1, 1, 1});
    C->name = 'C';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    root = insert_bst_sf(C, root);
    
    matrix_sf *D = evaluate_expr_sf('D', "(A+B)*C", root);
    expect_matrices_equal(D, 2, 2, (int[]){5, 5, 10, 10});
    
    free(D);
    free_bst_sf(root);
}

Test(student_tests, test_case5_transpose_in_expr) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    A->name = 'A';
    // For A*B' where A is 2x3, B' must be 3x2, so B must be 2x3
    matrix_sf *B = copy_matrix(2, 3, (int[]){1, 0, 1, 0, 1, 1});
    B->name = 'B';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    
    matrix_sf *C = evaluate_expr_sf('C', "A*B'", root);
    expect_matrices_equal(C, 2, 2, (int[]){4, 5, 10, 11});
    
    free(C);
    free_bst_sf(root);
}

Test(student_tests, test_case7_chained_ops) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 0, 0, 1});
    A->name = 'A';
    matrix_sf *B = copy_matrix(2, 2, (int[]){2, 3, 4, 5});
    B->name = 'B';
    matrix_sf *C = copy_matrix(2, 2, (int[]){1, 1, 1, 1});
    C->name = 'C';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    root = insert_bst_sf(C, root);
    
    matrix_sf *D = evaluate_expr_sf('D', "A*B+C", root);
    expect_matrices_equal(D, 2, 2, (int[]){3, 4, 5, 6});
    
    free(D);
    free_bst_sf(root);
}

Test(student_tests, test_case8_large_mult) {
    matrix_sf *A = copy_matrix(3, 4, (int[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
    matrix_sf *B = copy_matrix(4, 2, (int[]){1, 0, 0, 1, 1, 0, 0, 1});
    matrix_sf *C = mult_mats_sf(A, B);
    expect_matrices_equal(C, 3, 2, (int[]){4, 6, 12, 14, 20, 22});
    free(A);
    free(B);
    free(C);
}

Test(student_tests, test_case9_nested_parentheses) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    A->name = 'A';
    matrix_sf *B = copy_matrix(2, 2, (int[]){5, 6, 7, 8});
    B->name = 'B';
    matrix_sf *C = copy_matrix(2, 2, (int[]){1, 0, 0, 1});
    C->name = 'C';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    root = insert_bst_sf(C, root);
    
    matrix_sf *D = evaluate_expr_sf('D', "((A+B)'*C)'", root);
    expect_matrices_equal(D, 2, 2, (int[]){6, 8, 10, 12});
    
    free(D);
    free_bst_sf(root);
}

Test(student_tests, test_case10_all_ops) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    A->name = 'A';
    matrix_sf *B = copy_matrix(3, 2, (int[]){1, 0, 0, 1, 1, 1});
    B->name = 'B';
    matrix_sf *C = copy_matrix(2, 2, (int[]){2, 2, 2, 2});
    C->name = 'C';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    root = insert_bst_sf(C, root);
    
    matrix_sf *D = evaluate_expr_sf('D', "A*B+C", root);
    expect_matrices_equal(D, 2, 2, (int[]){6, 7, 12, 13});
    
    free(D);
    free_bst_sf(root);
}

Test(student_tests, test_case14_precedence) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 2, (int[]){1, 2, 3, 4});
    A->name = 'A';
    matrix_sf *B = copy_matrix(2, 2, (int[]){1, 0, 0, 1});
    B->name = 'B';
    matrix_sf *C = copy_matrix(2, 2, (int[]){2, 2, 2, 2});
    C->name = 'C';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    root = insert_bst_sf(C, root);
    
    matrix_sf *D = evaluate_expr_sf('D', "A'*B+C", root);
    expect_matrices_equal(D, 2, 2, (int[]){3, 5, 4, 6});
    
    free(D);
    free_bst_sf(root);
}

Test(student_tests, test_case15_max_complexity) {
    bst_sf *root = NULL;
    matrix_sf *A = copy_matrix(2, 3, (int[]){1, 2, 3, 4, 5, 6});
    A->name = 'A';
    matrix_sf *B = copy_matrix(3, 2, (int[]){1, 0, 0, 1, 1, 1});
    B->name = 'B';
    matrix_sf *C = copy_matrix(2, 2, (int[]){1, 1, 1, 1});
    C->name = 'C';
    matrix_sf *D = copy_matrix(2, 2, (int[]){2, 0, 0, 2});
    D->name = 'D';
    root = insert_bst_sf(A, root);
    root = insert_bst_sf(B, root);
    root = insert_bst_sf(C, root);
    root = insert_bst_sf(D, root);
    
    matrix_sf *E = evaluate_expr_sf('E', "((A*B)'+C)*D", root);
    expect_matrices_equal(E, 2, 2, (int[]){10, 22, 12, 24});
    
    free(E);
    free_bst_sf(root);
}
