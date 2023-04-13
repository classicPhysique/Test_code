#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    float average;
    int student_id;
    char subject[50];
    int score;
} Student;

typedef struct TreeNode {
    Student data;
    struct TreeNode* left, * right;
} TreeNode;


void menu();
void sort_menu();
TreeNode* insert_node(TreeNode* node, Student data, int (*comparator)(const void*, const void*));
TreeNode* delete_node(TreeNode* node, int student_id);
TreeNode* find_min(TreeNode* node);
void print_inorder(TreeNode* node);
void free_tree(TreeNode* node);
int compare_student_id(const void* a, const void* b);
int compare_name(const void* a, const void* b);
int compare_average(const void* a, const void* b);

int main() {
    FILE* file = fopen("students.txt", "r");
    if (!file) {
        printf("������ �� �� �����ϴ�.\n");
        return 1;
    }

    TreeNode* root_name = NULL;
    TreeNode* root_average = NULL;
    TreeNode* root_student_id = NULL;
    Student temp;
    

    //�о����
    while (fscanf(file, "%s %f %d %s %d", temp.name, &temp.average, &temp.student_id, temp.subject, &temp.score) == 5) {
        root_name = insert_node(root_name, temp, compare_name);
        root_average = insert_node(root_average, temp, compare_average);
        root_student_id = insert_node(root_student_id, temp, compare_student_id);
    }
    fclose(file);


    //����� �������̽� ����
    while (1) {
        printf("\n\n");
        menu();
        int choice;
        scanf("%d", &choice);
        getchar(); // ���� ����
        if (choice == 1) {
            while (1) {
                sort_menu();
                int sort_choice;
                scanf("%d", &sort_choice);
                getchar(); // ���� ����
                if (sort_choice == 1) {
                    printf("\n�̸��� ����:\n");
                    print_inorder(root_name);
                }
                else if (sort_choice == 2) {
                    printf("\n��ռ� ����:\n");
                    print_inorder(root_average);
                }
                else if (sort_choice == 3) {
                    printf("\n�й��� ����:\n");
                    print_inorder(root_student_id);
                }
                else {
                    break;
                }
                printf("\n���͸� ������ �޴��� ���ư��ϴ�.");
                getchar();
                break;
            }
        }
        else if (choice == 2) {
            Student new_student;
            printf("\n����� �߰�:\n");
            printf("�̸�: ");
            scanf("%s", new_student.name);
            printf("���: ");
            scanf("%f", &new_student.average);
            printf("�й�: ");
            scanf("%d", &new_student.student_id);
            printf("����: ");
            scanf("%s", new_student.subject);
            printf("����: ");
            scanf("%d", &new_student.score);
            getchar(); // ���� ����
            root_name = insert_node(root_name, new_student, compare_name);
            root_average = insert_node(root_average, new_student, compare_average);
            root_student_id = insert_node(root_student_id, new_student, compare_student_id);

            // ����� �߰� �� txt���Ͽ� ������Ʈ
            FILE* file = fopen("students.txt", "a");
            if (file) {
                fprintf(file, "\n%s %0.1f %d %s %d", new_student.name, new_student.average, new_student.student_id, new_student.subject, new_student.score);
                fclose(file);
            }
            else {
                printf("������ �� �� �����ϴ�.\n");
            }

            printf("\n����ڰ� �߰��Ǿ����ϴ�. ���͸� ������ �޴��� ���ư��ϴ�.");
            getchar();
        }
        else if (choice == 3) {
            // ���� ����
            int student_id;

            // ����ڿ��� ������ �й� �Է� ��û
            printf("\n����� ����:\n");
            printf("������ �й��� �Է��ϼ���: ");
            scanf("%d", &student_id);
            getchar(); // ���� ����

            // �й����� ��� ����
            root_name = delete_node(root_name, student_id);
            root_average = delete_node(root_average, student_id);
            root_student_id = delete_node(root_student_id, student_id);

            // ����� ���� �� txt ���� ������Ʈ
            FILE* file = fopen("students.txt", "r");
            FILE* temp_file = fopen("temp_students.txt", "w");

            // ������ ���������� ���ȴ��� Ȯ��
            if (file && temp_file) {
                Student temp_student;

                // ���Ͽ��� �����͸� �о��
                while (fscanf(file, "%s %f %d %s %d", temp_student.name, &temp_student.average, &temp_student.student_id, temp_student.subject, &temp_student.score) == 5) {
                    // ������ �й��� �ƴ� ��쿡�� temp_students.txt ���Ͽ� ���
                    if (temp_student.student_id != student_id) {
                        fprintf(temp_file, "%s %f %d %s %d\n", temp_student.name, temp_student.average, temp_student.student_id, temp_student.subject, temp_student.score);
                    }
                }

                // ���� �ݱ�
                fclose(file);
                fclose(temp_file);

                // ���� ���� ���� �� temp_students.txt ������ students.txt�� ����
                // ������ �����ִ»��·� �״�� �� ���,  ���� ��ũ���Ͱ� �ߺ��Ǿ� �������� ���� �����Ͱ� �ݺ��ؼ� ����
                // ���� ������ �����Ͽ�, �ٸ� ���� ��ũ���� ���. �߰��� ����ڸ� �Է�. 
                // ���� ���� ����
                remove("students.txt");
                rename("temp_students.txt", "students.txt");
            }
            else {
                printf("������ �� �� �����ϴ�.\n");
            }

            // Ʈ�� �޸� ����
            free_tree(root_name);
            free_tree(root_average);
            free_tree(root_student_id);

            // Ʈ�� �ʱ�ȭ
            root_name = NULL;
            root_average = NULL;
            root_student_id = NULL;

            // ���� �ٽ� ����
            file = fopen("students.txt", "r");

            // ������ ���������� ���ȴ��� Ȯ��
            if (file) {
                Student temp_student;

                // ���Ͽ��� �����͸� �о�� Ʈ���� ����
                while (fscanf(file, "%s %f %d %s %d", temp_student.name, &temp_student.average, &temp_student.student_id, temp_student.subject, &temp_student.score) == 5) {
                    root_name = insert_node(root_name, temp_student, compare_name);
                    root_average = insert_node(root_average, temp_student, compare_average);
                    root_student_id = insert_node(root_student_id, temp_student, compare_student_id);
                }

                // ���� �ݱ�
                fclose(file);
            }
            else {
                printf("������ �� �� �����ϴ�.\n");
            }

            // ����� ���� �Ϸ� �޽��� ���
            printf("\n����ڰ� �����Ǿ����ϴ�. ���͸� ������ �޴��� ���ư��ϴ�.");
            getchar();
        }
        else if (choice == 4) {
            printf("\n���α׷��� �����մϴ�.\n");
            break;
        }
        else {
            printf("\n�߸��� �Է��Դϴ�. �޴��� ���ư��ϴ�.\n");
        }
    }


    free_tree(root_name);
    free_tree(root_average);
    free_tree(root_student_id);

    return 0;
}

void menu() {
    printf("MENU\n");
    printf("1. ����� ����\n");
    printf("2. ����� �߰�\n");
    printf("3. ����� ����\n");
    printf("4. ����\n");
    printf("������ �޴� ��ȣ�� �Է��ϼ���: ");
}

void sort_menu() {
    printf("\n����� ����\n");
    printf("1. �̸��� ����\n");
    printf("2. ��ռ� ����\n");
    printf("3. �й��� ����\n");
    printf("������ ���� ��ȣ�� �Է��ϼ���: ");
}

// ���� Ž�� Ʈ���� ��带 �����ϴ� �Լ�
TreeNode* insert_node(TreeNode* node, Student data, int (*comparator)(const void*, const void*)) {
    // ���� ���: ��尡 ����ִ� ��� �� ��带 �����ϰ� �����͸� ����
    if (node == NULL) {
        TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
        new_node->data = data;
        new_node->left = new_node->right = NULL;
        return new_node;
    }

    // �����͸� ���Ͽ� ���� �Ǵ� ������ ����Ʈ���� ����
    if (comparator(&data, &node->data) < 0) {
        node->left = insert_node(node->left, data, comparator);
    }
    else {
        node->right = insert_node(node->right, data, comparator);
    }

    return node;
}

// ���� Ž�� Ʈ������ ��带 �����ϴ� �Լ�
TreeNode* delete_node(TreeNode* root, int student_id) {
    TreeNode* parent, * current, * temp;
    parent = NULL;
    current = root;

    // �й��� ã�� ���� Ʈ���� Ž��
    while (current != NULL) {
        if (current->data.student_id == student_id) {
            break;
        }
        parent = current;
        if (student_id < current->data.student_id) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    // �й��� ���� ���, ��Ʈ�� ��ȯ
    if (current == NULL) {
        return root;
    }

    // ������ ����� �ڽ� ��尡 ���ų� �ϳ��� �ִ� ��� ó��
    if (current->left == NULL) {
        temp = current->right;
    }
    else if (current->right == NULL) {
        temp = current->left;
    }
    // �� �ڽ� ��尡 �ִ� ��� ó��
    else {
        TreeNode* temp_parent = current;
        temp = current->right;
        while (temp->left != NULL) {
            temp_parent = temp;
            temp = temp->left;
        }
        if (temp_parent != current) {
            temp_parent->left = temp->right;
            temp->right = current->right;
        }
        temp->left = current->left;
    }

    // ������ ��尡 ��Ʈ�� ��� ó��
    if (parent == NULL) {
        root = temp;
    }
    // ������ ��尡 ��Ʈ�� �ƴ� ��� ó��
    else if (parent->left == current) {
        parent->left = temp;
    }
    else {
        parent->right = temp;
    }

    // ������ ��� �޸� ����
    free(current);
    current = NULL;
    return root;
}

// �ּ� ��� ã�� �Լ�
TreeNode* find_min(TreeNode* node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

//Ʈ���� ��ȸ�ϸ� ����ϴ� �Լ�
void print_inorder(TreeNode* node) {
    if (node) {
        print_inorder(node->left);
        printf("%s %f %d %s %d\n", node->data.name,node->data.average , node->data.student_id, node->data.subject, node->data.score);
        print_inorder(node->right);
    }
}


//Ʈ���� ���� �Ҵ��� �޸𸮿��� �����ϴ� �Լ�
void free_tree(TreeNode* node) {
    if (node) {
        free_tree(node->left);
        free_tree(node->right);
        free(node);
    }
}

// �й��� �������� �� �л��� ���ϴ� �Լ�
int compare_student_id(const void* a, const void* b) {
    Student* student_a = (Student*)a;
    Student* student_b = (Student*)b;
    return student_a->student_id - student_b->student_id;
}

// �̸��� �������� �� �л��� ���ϴ� �Լ�
int compare_name(const void* a, const void* b) {
    Student* student_a = (Student*)a;
    Student* student_b = (Student*)b;
    return strcmp(student_a->name, student_b->name);
}

// ��� ������ �������� �� �л��� ���ϴ� �Լ�
int compare_average(const void* a, const void* b) {
    Student* student_a = (Student*)a;
    Student* student_b = (Student*)b;
    return (student_a->average < student_b->average) - (student_a->average > student_b->average);
}


//�� �Լ����� ���� Ž�� Ʈ���� ��带 �����ϰų� Ž���� �� ���Ǵ� �� �Լ��Դϴ�. 
//���� Ž�� Ʈ���� �����͸� ���ĵ� ���·� �����ϹǷ�, ��带 �����ϰų� Ž���� �� �� �Լ��� ����Ͽ� �����͸� ���ĵ� ������� ������ �� �ֽ��ϴ�. 
//�� ���, compare_student_id, compare_name, compare_average �Լ��� ����Ͽ� �й�, �̸�, ��� ������ �������� ���ĵ� ���� Ž�� Ʈ���� ������ �� �ֽ��ϴ�. 
//�̷��� ������ ���� Ž�� Ʈ���� ���� �˻� �� ���ĵ� ����� ������ �� �ֽ��ϴ�.
