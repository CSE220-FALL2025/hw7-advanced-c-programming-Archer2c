#define _POSIX_C_SOURCE 200809L
#include "hw7.h"

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    if (root == NULL) {
        bst_sf *node = malloc(sizeof(bst_sf));
        node->left_child = NULL;
        node->right_child = NULL;
        node->mat = mat;
        return node;
    }
    
    bst_sf *current = root;
    int inserted = 0;
    while (!inserted) {
        if (mat->name < current->mat->name) {
            if (current->left_child == NULL) {
                bst_sf *node = malloc(sizeof(bst_sf));
                node->left_child = NULL;
                node->right_child = NULL;
                node->mat = mat;
                current->left_child = node;
                inserted = 1;
            } else {
                current = current->left_child;
            }
        } else {
            if (current->right_child == NULL) {
                bst_sf *node = malloc(sizeof(bst_sf));
                node->left_child = NULL;
                node->right_child = NULL;
                node->mat = mat;
                current->right_child = node;
                inserted = 1;
            } else {
                current = current->right_child;
            }
        }
    }
    
    return root;
}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    if (root == NULL) {
        return NULL;
    }
    
    bst_sf *current = root;
    while (current != NULL) {
        if (name == current->mat->name) {
            return current->mat;
        }
        if (name < current->mat->name) {
            current = current->left_child;
        } else {
            current = current->right_child;
        }
    }
    
    return NULL;
}

void free_bst_sf(bst_sf *root) {
    if (root == NULL) {
        return;
    }
    
    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);
    free(root->mat);
    free(root);
}

matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    unsigned int rows = mat1->num_rows;
    unsigned int cols = mat1->num_cols;
    
    matrix_sf *ans = malloc(sizeof(matrix_sf) + rows * cols * sizeof(int));
    if (ans == NULL) {
        return NULL;
    }
    ans->name = '?';
    ans->num_rows = rows;
    ans->num_cols = cols;
    
    unsigned int total = rows * cols;
    for (unsigned int idx = 0; idx < total; idx++) {
        ans->values[idx] = mat1->values[idx] + mat2->values[idx];
    }
    
    return ans;
}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    unsigned int r1 = mat1->num_rows;
    unsigned int c1 = mat1->num_cols;
    unsigned int c2 = mat2->num_cols;
    
    matrix_sf *output = malloc(sizeof(matrix_sf) + r1 * c2 * sizeof(int));
    output->name = '?';
    output->num_rows = r1;
    output->num_cols = c2;
    
    for (unsigned int i = 0; i < r1; i++) {
        for (unsigned int j = 0; j < c2; j++) {
            output->values[i * c2 + j] = 0;
        }
    }
    
    for (unsigned int i = 0; i < r1; i++) {
        for (unsigned int j = 0; j < c2; j++) {
            int sum = 0;
            for (unsigned int k = 0; k < c1; k++) {
                sum += mat1->values[i * c1 + k] * mat2->values[k * c2 + j];
            }
            output->values[i * c2 + j] = sum;
        }
    }
    
    return output;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    unsigned int old_rows = mat->num_rows;
    unsigned int old_cols = mat->num_cols;
    unsigned int new_rows = old_cols;
    unsigned int new_cols = old_rows;
    
    matrix_sf *t = malloc(sizeof(matrix_sf) + new_rows * new_cols * sizeof(int));
    t->name = '?';
    t->num_rows = new_rows;
    t->num_cols = new_cols;
    
    for (unsigned int i = 0; i < old_rows; i++) {
        for (unsigned int j = 0; j < old_cols; j++) {
            unsigned int old_pos = i * old_cols + j;
            unsigned int new_pos = j * old_rows + i;
            t->values[new_pos] = mat->values[old_pos];
        }
    }
    
    return t;
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    const char *s = expr;
    while (*s != '\0' && isspace(*s)) {
        s++;
    }
    
    unsigned int rows = 0;
    while (*s != '\0' && isdigit(*s)) {
        rows = rows * 10 + (*s - '0');
        s++;
    }
    
    while (*s != '\0' && isspace(*s)) {
        s++;
    }
    
    unsigned int cols = 0;
    while (*s != '\0' && isdigit(*s)) {
        cols = cols * 10 + (*s - '0');
        s++;
    }
    
    while (*s != '\0' && isspace(*s)) {
        s++;
    }
    if (*s == '[') {
        s++;
    }
    while (*s != '\0' && isspace(*s)) {
        s++;
    }
    
    matrix_sf *m = malloc(sizeof(matrix_sf) + rows * cols * sizeof(int));
    m->name = name;
    m->num_rows = rows;
    m->num_cols = cols;
    
    int pos = 0;
    for (unsigned int row = 0; row < rows; row++) {
        for (unsigned int col = 0; col < cols; col++) {
            while (*s != '\0' && isspace(*s)) {
                s++;
            }
            
            int is_negative = 0;
            if (*s == '-') {
                is_negative = 1;
                s++;
                while (*s != '\0' && isspace(*s)) {
                    s++;
                }
            }
            
            int num = 0;
            while (*s != '\0' && isdigit(*s)) {
                num = num * 10 + (*s - '0');
                s++;
            }
            
            if (is_negative == 1) {
                m->values[pos] = -num;
            } else {
                m->values[pos] = num;
            }
            pos = pos + 1;
            
            while (*s != '\0' && isspace(*s)) {
                s++;
            }
            if (*s == ';') {
                s++;
            }
        }
    }
    
    while (*s != '\0' && *s != ']') {
        s++;
    }
    
    return m;
}

char* infix2postfix_sf(char *infix) {
    int n = strlen(infix);
    char *post = malloc((n + 1) * sizeof(char));
    int out_pos = 0;
    
    char *op_stack = malloc(n * sizeof(char));
    int top = -1;
    
    int i = 0;
    while (infix[i] != '\0') {
        if (isspace(infix[i])) {
            i++;
            continue;
        }
        
        if (isupper(infix[i])) {
            post[out_pos] = infix[i];
            out_pos++;
            i++;
            
            int j = i;
            while (infix[j] != '\0' && isspace(infix[j])) j++;
            if (infix[j] == '\'') {
                post[out_pos] = '\'';
                out_pos++;
                i = j + 1;
            }
            continue;
        }
        
        if (infix[i] == '(') {
            top++;
            op_stack[top] = infix[i];
            i++;
            continue;
        }
        
        if (infix[i] == ')') {
            while (top >= 0) {
                if (op_stack[top] == '(') {
                    top--;
                    break;
                }
                post[out_pos] = op_stack[top];
                out_pos++;
                top--;
            }
            i++;
            
            int j = i;
            while (infix[j] != '\0' && isspace(infix[j])) j++;
            if (infix[j] == '\'') {
                post[out_pos] = '\'';
                out_pos++;
                i = j + 1;
            }
            continue;
        }
        
        if (infix[i] == '\'') {
            post[out_pos] = '\'';
            out_pos++;
            i++;
            continue;
        }
        
        if (infix[i] == '+' || infix[i] == '*') {
            int prec;
            if (infix[i] == '*') {
                prec = 2;
            } else {
                prec = 1;
            }
            
            while (top >= 0) {
                if (op_stack[top] == '(') {
                    break;
                }
                int stack_prec;
                if (op_stack[top] == '*') {
                    stack_prec = 2;
                } else if (op_stack[top] == '+') {
                    stack_prec = 1;
                } else {
                    break;
                }
                
                if (stack_prec < prec) {
                    break;
                }
                post[out_pos] = op_stack[top];
                out_pos++;
                top--;
            }
            
            top++;
            op_stack[top] = infix[i];
            i++;
            continue;
        }
        
        i++;
    }
    
    while (top >= 0) {
        if (op_stack[top] != '(') {
            post[out_pos] = op_stack[top];
            out_pos++;
        }
        top--;
    }
    
    post[out_pos] = '\0';
    free(op_stack);
    
    return post;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    char *post = infix2postfix_sf(expr);
    
    int n = strlen(post);
    matrix_sf **mat_stack = malloc(n * sizeof(matrix_sf*));
    int top = -1;
    
    int i = 0;
    while (post[i] != '\0') {
        if (isspace(post[i])) {
            i++;
            continue;
        }
        
        if (isupper(post[i])) {
            matrix_sf *found = find_bst_sf(post[i], root);
            top++;
            mat_stack[top] = found;
            i++;
            continue;
        }
        
        if (post[i] == '\'') {
            if (top >= 0) {
                matrix_sf *m = mat_stack[top];
                top--;
                
                matrix_sf *trans = transpose_mat_sf(m);
                if (trans != NULL) {
                    trans->name = '?';
                }
                
                int should_free = 0;
                if (m->name < 'A' || m->name > 'Z') {
                    should_free = 1;
                }
                if (should_free) {
                    free(m);
                }
                
                top++;
                mat_stack[top] = trans;
            }
            i++;
            continue;
        }
        
        if (post[i] == '+' || post[i] == '*') {
            if (top >= 1) {
                matrix_sf *right = mat_stack[top];
                top--;
                matrix_sf *left = mat_stack[top];
                top--;
                
                matrix_sf *res = NULL;
                if (post[i] == '+') {
                    res = add_mats_sf(left, right);
                } else if (post[i] == '*') {
                    res = mult_mats_sf(left, right);
                }
                
                if (res != NULL) {
                    res->name = '?';
                }
                
                int left_should_free = 0;
                if (left->name < 'A' || left->name > 'Z') {
                    left_should_free = 1;
                }
                int right_should_free = 0;
                if (right->name < 'A' || right->name > 'Z') {
                    right_should_free = 1;
                }
                
                if (left_should_free == 1) {
                    free(left);
                }
                if (right_should_free == 1) {
                    free(right);
                }
                
                top++;
                mat_stack[top] = res;
            }
            i++;
            continue;
        }
        
        i++;
    }
    
    matrix_sf *final = NULL;
    if (top >= 0) {
        final = mat_stack[top];
        final->name = name;
    }
    
    free(mat_stack);
    free(post);
    
    return final;
}

matrix_sf *execute_script_sf(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return NULL;
    }
    
    bst_sf *tree = NULL;
    matrix_sf *final = NULL;
    char line_buf[MAX_LINE_LEN + 1];
    
    while (fgets(line_buf, MAX_LINE_LEN + 1, f) != NULL) {
        int len = strlen(line_buf);
        if (len > 0 && line_buf[len - 1] == '\n') {
            line_buf[len - 1] = '\0';
            len--;
        }
        
        if (len == 0 || line_buf[0] == '\0') {
            continue;
        }
        
        int start = 0;
        while (start < len && isspace(line_buf[start])) start++;
        
        if (start >= len) {
            continue;
        }
        
        if (!isupper(line_buf[start])) {
            continue;
        }
        char var_name = line_buf[start];
        start++;
        
        while (start < len && isspace(line_buf[start])) start++;
        if (start >= len || line_buf[start] != '=') {
            continue;
        }
        start++;
        while (start < len && isspace(line_buf[start])) start++;
        
        int has_bracket = 0;
        int idx = start;
        while (idx < len) {
            if (line_buf[idx] == '[') {
                has_bracket = 1;
                break;
            }
            idx++;
        }
        
        if (has_bracket) {
            matrix_sf *new_mat = create_matrix_sf(var_name, &line_buf[start]);
            if (new_mat != NULL) {
                tree = insert_bst_sf(new_mat, tree);
                final = new_mat;
            }
        } else {
            int end = len - 1;
            while (end >= start) {
                if (isspace(line_buf[end])) {
                    line_buf[end] = '\0';
                    end--;
                } else {
                    break;
                }
            }
            
            matrix_sf *new_mat = evaluate_expr_sf(var_name, &line_buf[start], tree);
            if (new_mat != NULL) {
                tree = insert_bst_sf(new_mat, tree);
                final = new_mat;
            }
        }
    }
    
    fclose(f);
    
    return final;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
