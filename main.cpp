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
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    TreeNode* root_name = NULL;
    TreeNode* root_average = NULL;
    TreeNode* root_student_id = NULL;
    Student temp;
    

    //읽어오기
    while (fscanf(file, "%s %f %d %s %d", temp.name, &temp.average, &temp.student_id, temp.subject, &temp.score) == 5) {
        root_name = insert_node(root_name, temp, compare_name);
        root_average = insert_node(root_average, temp, compare_average);
        root_student_id = insert_node(root_student_id, temp, compare_student_id);
    }
    fclose(file);


    //사용자 인터페이스 구성
    while (1) {
        printf("\n\n");
        menu();
        int choice;
        scanf("%d", &choice);
        getchar(); // 엔터 제거
        if (choice == 1) {
            while (1) {
                sort_menu();
                int sort_choice;
                scanf("%d", &sort_choice);
                getchar(); // 엔터 제거
                if (sort_choice == 1) {
                    printf("\n이름순 정렬:\n");
                    print_inorder(root_name);
                }
                else if (sort_choice == 2) {
                    printf("\n평균순 정렬:\n");
                    print_inorder(root_average);
                }
                else if (sort_choice == 3) {
                    printf("\n학번순 정렬:\n");
                    print_inorder(root_student_id);
                }
                else {
                    break;
                }
                printf("\n엔터를 누르면 메뉴로 돌아갑니다.");
                getchar();
                break;
            }
        }
        else if (choice == 2) {
            Student new_student;
            printf("\n사용자 추가:\n");
            printf("이름: ");
            scanf("%s", new_student.name);
            printf("평균: ");
            scanf("%f", &new_student.average);
            printf("학번: ");
            scanf("%d", &new_student.student_id);
            printf("과목: ");
            scanf("%s", new_student.subject);
            printf("점수: ");
            scanf("%d", &new_student.score);
            getchar(); // 엔터 제거
            root_name = insert_node(root_name, new_student, compare_name);
            root_average = insert_node(root_average, new_student, compare_average);
            root_student_id = insert_node(root_student_id, new_student, compare_student_id);

            // 사용자 추가 후 txt파일에 업데이트
            FILE* file = fopen("students.txt", "a");
            if (file) {
                fprintf(file, "\n%s %0.1f %d %s %d", new_student.name, new_student.average, new_student.student_id, new_student.subject, new_student.score);
                fclose(file);
            }
            else {
                printf("파일을 열 수 없습니다.\n");
            }

            printf("\n사용자가 추가되었습니다. 엔터를 누르면 메뉴로 돌아갑니다.");
            getchar();
        }
        else if (choice == 3) {
            // 변수 선언
            int student_id;

            // 사용자에게 삭제할 학번 입력 요청
            printf("\n사용자 삭제:\n");
            printf("삭제할 학번을 입력하세요: ");
            scanf("%d", &student_id);
            getchar(); // 엔터 제거

            // 학번으로 노드 삭제
            root_name = delete_node(root_name, student_id);
            root_average = delete_node(root_average, student_id);
            root_student_id = delete_node(root_student_id, student_id);

            // 사용자 삭제 후 txt 파일 업데이트
            FILE* file = fopen("students.txt", "r");
            FILE* temp_file = fopen("temp_students.txt", "w");

            // 파일이 정상적으로 열렸는지 확인
            if (file && temp_file) {
                Student temp_student;

                // 파일에서 데이터를 읽어옴
                while (fscanf(file, "%s %f %d %s %d", temp_student.name, &temp_student.average, &temp_student.student_id, temp_student.subject, &temp_student.score) == 5) {
                    // 삭제된 학번이 아닌 경우에만 temp_students.txt 파일에 기록
                    if (temp_student.student_id != student_id) {
                        fprintf(temp_file, "%s %f %d %s %d\n", temp_student.name, temp_student.average, temp_student.student_id, temp_student.subject, temp_student.score);
                    }
                }

                // 파일 닫기
                fclose(file);
                fclose(temp_file);

                // 원래 파일 삭제 후 temp_students.txt 파일을 students.txt로 변경
                // 파일을 열고있는상태로 그대로 쓸 경우,  파일 디스크립터가 중복되어 수만건의 같은 데이터가 반복해서 쓰임
                // 기존 파일을 복사하여, 다른 파일 디스크립터 사용. 추가된 사용자를 입력. 
                // 기존 파일 삭제
                remove("students.txt");
                rename("temp_students.txt", "students.txt");
            }
            else {
                printf("파일을 열 수 없습니다.\n");
            }

            // 트리 메모리 해제
            free_tree(root_name);
            free_tree(root_average);
            free_tree(root_student_id);

            // 트리 초기화
            root_name = NULL;
            root_average = NULL;
            root_student_id = NULL;

            // 파일 다시 열기
            file = fopen("students.txt", "r");

            // 파일이 정상적으로 열렸는지 확인
            if (file) {
                Student temp_student;

                // 파일에서 데이터를 읽어와 트리에 삽입
                while (fscanf(file, "%s %f %d %s %d", temp_student.name, &temp_student.average, &temp_student.student_id, temp_student.subject, &temp_student.score) == 5) {
                    root_name = insert_node(root_name, temp_student, compare_name);
                    root_average = insert_node(root_average, temp_student, compare_average);
                    root_student_id = insert_node(root_student_id, temp_student, compare_student_id);
                }

                // 파일 닫기
                fclose(file);
            }
            else {
                printf("파일을 열 수 없습니다.\n");
            }

            // 사용자 삭제 완료 메시지 출력
            printf("\n사용자가 삭제되었습니다. 엔터를 누르면 메뉴로 돌아갑니다.");
            getchar();
        }
        else if (choice == 4) {
            printf("\n프로그램을 종료합니다.\n");
            break;
        }
        else {
            printf("\n잘못된 입력입니다. 메뉴로 돌아갑니다.\n");
        }
    }


    free_tree(root_name);
    free_tree(root_average);
    free_tree(root_student_id);

    return 0;
}

void menu() {
    printf("MENU\n");
    printf("1. 사용자 정렬\n");
    printf("2. 사용자 추가\n");
    printf("3. 사용자 삭제\n");
    printf("4. 종료\n");
    printf("선택할 메뉴 번호를 입력하세요: ");
}

void sort_menu() {
    printf("\n사용자 정렬\n");
    printf("1. 이름순 정렬\n");
    printf("2. 평균순 정렬\n");
    printf("3. 학번순 정렬\n");
    printf("선택할 정렬 번호를 입력하세요: ");
}

// 이진 탐색 트리에 노드를 삽입하는 함수
TreeNode* insert_node(TreeNode* node, Student data, int (*comparator)(const void*, const void*)) {
    // 기저 사례: 노드가 비어있는 경우 새 노드를 생성하고 데이터를 삽입
    if (node == NULL) {
        TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
        new_node->data = data;
        new_node->left = new_node->right = NULL;
        return new_node;
    }

    // 데이터를 비교하여 왼쪽 또는 오른쪽 서브트리에 삽입
    if (comparator(&data, &node->data) < 0) {
        node->left = insert_node(node->left, data, comparator);
    }
    else {
        node->right = insert_node(node->right, data, comparator);
    }

    return node;
}

// 이진 탐색 트리에서 노드를 삭제하는 함수
TreeNode* delete_node(TreeNode* root, int student_id) {
    TreeNode* parent, * current, * temp;
    parent = NULL;
    current = root;

    // 학번을 찾기 위해 트리를 탐색
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

    // 학번이 없는 경우, 루트를 반환
    if (current == NULL) {
        return root;
    }

    // 삭제할 노드의 자식 노드가 없거나 하나만 있는 경우 처리
    if (current->left == NULL) {
        temp = current->right;
    }
    else if (current->right == NULL) {
        temp = current->left;
    }
    // 두 자식 노드가 있는 경우 처리
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

    // 삭제할 노드가 루트인 경우 처리
    if (parent == NULL) {
        root = temp;
    }
    // 삭제할 노드가 루트가 아닌 경우 처리
    else if (parent->left == current) {
        parent->left = temp;
    }
    else {
        parent->right = temp;
    }

    // 삭제할 노드 메모리 해제
    free(current);
    current = NULL;
    return root;
}

// 최소 노드 찾는 함수
TreeNode* find_min(TreeNode* node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

//트리를 순회하며 출력하는 함수
void print_inorder(TreeNode* node) {
    if (node) {
        print_inorder(node->left);
        printf("%s %f %d %s %d\n", node->data.name,node->data.average , node->data.student_id, node->data.subject, node->data.score);
        print_inorder(node->right);
    }
}


//트리의 동적 할당을 메모리에서 해제하는 함수
void free_tree(TreeNode* node) {
    if (node) {
        free_tree(node->left);
        free_tree(node->right);
        free(node);
    }
}

// 학번을 기준으로 두 학생을 비교하는 함수
int compare_student_id(const void* a, const void* b) {
    Student* student_a = (Student*)a;
    Student* student_b = (Student*)b;
    return student_a->student_id - student_b->student_id;
}

// 이름을 기준으로 두 학생을 비교하는 함수
int compare_name(const void* a, const void* b) {
    Student* student_a = (Student*)a;
    Student* student_b = (Student*)b;
    return strcmp(student_a->name, student_b->name);
}

// 평균 점수를 기준으로 두 학생을 비교하는 함수
int compare_average(const void* a, const void* b) {
    Student* student_a = (Student*)a;
    Student* student_b = (Student*)b;
    return (student_a->average < student_b->average) - (student_a->average > student_b->average);
}


//이 함수들은 이진 탐색 트리의 노드를 삽입하거나 탐색할 때 사용되는 비교 함수입니다. 
//이진 탐색 트리는 데이터를 정렬된 상태로 저장하므로, 노드를 삽입하거나 탐색할 때 비교 함수를 사용하여 데이터를 정렬된 순서대로 정렬할 수 있습니다. 
//이 경우, compare_student_id, compare_name, compare_average 함수를 사용하여 학번, 이름, 평균 점수를 기준으로 정렬된 이진 탐색 트리를 구성할 수 있습니다. 
//이렇게 구성된 이진 탐색 트리는 빠른 검색 및 정렬된 결과를 제공할 수 있습니다.
