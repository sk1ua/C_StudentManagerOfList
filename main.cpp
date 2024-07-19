//
// Created by 15857 on 24-7-15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"

#define N 100

STU* head = NULL;
char NowFileName[255] = "StudentInit.dat";
//��ѧ����Ϣ����ϵͳ
void Input(const char* filename, STU*& head) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("�޷����ļ�");
        return;
    }

    STU temp;
    int count = 0;
    while (fread(&temp, sizeof(STU), 1, file) == 1) {
        STU* newNode = (STU*)malloc(sizeof(STU));
        *newNode = temp;
        newNode->next = head;
        head = newNode;
        count++;
    }

    printf("�ɹ����� %d ��ѧ����Ϣ\n", count);

    fclose(file);
}

//��ѧ����Ϣд���ļ�
void InputToFile(const char* filename, STU* head) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("�޷����ļ�");
        return;
    }

    STU* current = head;
    while (current != NULL) {
        if (fwrite(current, sizeof(STU), 1, file) != 1) {
            perror("д���ļ�ʱ����");
            break;
        }
        current = current->next;
    }

    fclose(file);
}

// ��׼�����ʽ
void OutOneNode(STU *p) {
    if (p != NULL) {
        printf("| %-10s | %-20s | %-20s | %-6d | %-6d | %-6d | %-6d | %-6d |\n",
               p->ID, p->name, p->major, p->classNo, p->score[0], p->score[1], p->score[2],
               p->score[0] + p->score[1] + p->score[2]);
    } else {
        printf("�ýڵ�Ϊ�ա�\n");
    }
}

// ��׼�����ͷ
void Outputtitle() {
    printf("|     ID     |         Name         |        Major         | Class  | Score1 | Score2 | Score3 | Total  |\n");
}
// ��ʾ����ѧ����Ϣ
void Output(STU *head) {
    Outputtitle();
    STU* current = head;
    while (current != NULL) {
        OutOneNode(current);
        current = current->next;
    }
}
// ��ѧ������ѧ����Ϣ
void SortByID(STU*& head) {
    if (head == NULL || head->next == NULL) return;

    STU *sorted = NULL, *current = head;
    while (current != NULL) {
        STU* next = current->next;
        if (sorted == NULL || strcmp(current->ID, sorted->ID) < 0) {
            current->next = sorted;
            sorted = current;
        } else {
            STU* temp = sorted;
            while (temp->next != NULL && strcmp(temp->next->ID, current->ID) < 0) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}
// ͳ��ѧ������
int countStudents(STU* head) {
    int count = 0;
    STU* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}
// ����ѧ����Ϣ
void Save(STU*& head, int n) {
    for (int i = 0; i < n; i++) {
        STU* newNode = (STU*)malloc(sizeof(STU));
        if (newNode == NULL) {
            printf("�ڴ����ʧ��\n");
            return;
        }

        printf("����ѧ����Ϣ: \n");
        printf("ѧ��: ");
        scanf("%s", newNode->ID);
        getchar();
        printf("����: ");
        scanf("%s", newNode->name);
        getchar();
        printf("רҵ: ");
        scanf("%s", newNode->major);
        getchar();
        printf("�༶: ");
        scanf("%d", &newNode->classNo);
        getchar();
        printf("�γ�1�ɼ�: ");
        scanf("%d", &newNode->score[0]);
        getchar();
        printf("�γ�2�ɼ�: ");
        scanf("%d", &newNode->score[1]);
        getchar();
        printf("�γ�3�ɼ�: ");
        scanf("%d", &newNode->score[2]);
        getchar();

        newNode->next = head;
        head = newNode;
    }

    // д���ļ�
    InputToFile("studentInit.dat", head);
    printf("¼��ѧ����Ϣ�ɹ�\n");

    SortByID(head);
}
// �����ܷ���ߵ�ѧ��
STU* Max(STU* head) {
    if (head == NULL) return NULL;

    int maxScore = -1;
    STU* maxListHead = NULL;
    STU* maxListTail = NULL;

    for (STU* current = head; current != NULL; current = current->next) {
        int currentScore = current->score[0] + current->score[1] + current->score[2];
        if (currentScore > maxScore) {
            maxScore = currentScore;
        }
    }

    for (STU* current = head; current != NULL; current = current->next) {
        int currentScore = current->score[0] + current->score[1] + current->score[2];
        if (currentScore == maxScore) {
            STU* newNode = (STU*)malloc(sizeof(STU));
            if (newNode == NULL) {
                printf("�ڴ����ʧ��\n");
                while (maxListHead != NULL) {
                    STU* temp = maxListHead;
                    maxListHead = maxListHead->next;
                    free(temp);
                }
                return NULL;
            }
            *newNode = *current;
            newNode->next = NULL;

            if (maxListHead == NULL) {
                maxListHead = newNode;
                maxListTail = newNode;
            } else {
                maxListTail->next = newNode;
                maxListTail = newNode;
            }
        }
    }

    return maxListHead;
}
// ͨ��ѧ�Ų���ѧ��
STU *Search_num(STU *head, char *num) {
    STU *current = head;
    while (current != NULL) {
        if (strcmp(current->ID, num) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
// ͨ����������ѧ��
STU *Search_name(STU *head, char *name) {
    STU *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
// ͨ��ѧ��ɾ��ѧ��
void Delete_num(STU* &head, char* num) {
    STU* current = head;
    STU* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->ID, num) == 0) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("ѧ��Ϊ %s ��ѧ����Ϣ��ɾ����\n", num);
            InputToFile(NowFileName, head); // �����º������д���ļ�
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����\n", num);
}
// ͨ������ɾ��ѧ��
void Delete_name(STU* &head, char* name) {
    STU* current = head;
    STU* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("����Ϊ %s ��ѧ����Ϣ��ɾ����\n", name);
            InputToFile(NowFileName, head); // �����º������д���ļ�
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("δ�ҵ�����Ϊ %s ��ѧ����\n", name);
}

// �޸�ѧ����Ϣ
void ChangeStudentInfo(STU* found) {
    if (found != NULL) {
        Outputtitle();
        OutOneNode(found);
        printf("�����µ�ѧ����Ϣ: \n");
        printf("ѧ��: ");
        scanf("%s", found->ID);
        getchar();
        printf("����: ");
        scanf("%s", found->name);
        getchar();
        printf("רҵ: ");
        scanf("%s", found->major);
        getchar();
        printf("�༶: ");
        scanf("%d", &found->classNo);
        getchar();
        printf("�γ�1�ɼ�: ");
        scanf("%d", &found->score[0]);
        getchar();
        printf("�γ�2�ɼ�: ");
        scanf("%d", &found->score[1]);
        getchar();
        printf("�γ�3�ɼ�: ");
        scanf("%d", &found->score[2]);
        getchar();

        printf("ѧ����Ϣ�Ѹ��¡�\n");
        Outputtitle();
        OutOneNode(found);
    }
}
// ������ѧ����Ϣ
void InsertList(STU*& head, STU stu) {
    STU* newNode = (STU*)malloc(sizeof(STU));
    if (newNode == NULL) {
        printf("�ڴ����ʧ��\n");
        return;
    }
    *newNode = stu;
    newNode->next = NULL;

    if (head == NULL || strcmp(head->ID, newNode->ID) > 0) {
        newNode->next = head;
        head = newNode;
    } else {
        STU* current = head;
        while (current->next != NULL && strcmp(current->next->ID, newNode->ID) < 0) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    InputToFile(NowFileName, head);
}
// ����ָ��רҵ��ĳ�ſγ̳ɼ�С��ĳ��������ѧ��
STU *Search_major_subject_score(STU *head, char *major, int subject, int grade) {
    STU *newListHead = NULL, *newListTail = NULL;
    while (head != NULL) {
        if (strcmp(head->major, major) == 0 && head->score[subject] < grade) {
            STU *newNode = (STU *)malloc(sizeof(STU));
            *newNode = *head; // Copy student data
            newNode->next = NULL;
            if (newListHead == NULL) {
                newListHead = newNode;
                newListTail = newNode;
            } else {
                newListTail->next = newNode;
                newListTail = newNode;
            }
        }
        head = head->next;
    }
    return newListHead;
}
// ��ѧ����Ϣ���浽�ļ�
void Save(STU *head,char* filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("�޷����ļ�");
        return;
    }
    while (head != NULL) {
        fwrite(head, sizeof(STU), 1, file);
        head = head->next;
    }
    fclose(file);
}
// ɾ��ָ���༶��ĳ�ſγ̳ɼ�С��ĳ��������ѧ��
void Delete_class_subject_score(STU *&head, int classno, int subject, int grade) {
    STU *current = head, *prev = NULL;
    while (current != NULL) {
        if (current->classNo == classno && current->score[subject] < grade) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            STU *temp = current;
            current = current->next;
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
    printf("�Ƿ����Ϊ���ļ�(y / n):");
    char c;
    int read = scanf("%c",&c);
    if(read != 1) {
        printf("�����������������\n");
        while (getchar() != '\n');
    }
    if(c == 'y'){
        char filename[255];
        printf("�������ļ�����: ");
        scanf("%s", filename);
        Save(head,filename);
    }else if(c == 'n'){
        printf("δ����\n");
    }else{
        printf("�Ƿ�����");
    }
}
// ��ƽ��������ѧ����Ϣ
void SortByAverageScore(STU*& head) {
    if (head == NULL || head->next == NULL) return;

    STU *current = head, *maxNode, *prev, *maxPrev, *temp;
    for (; current != NULL && current->next != NULL; current = current->next) {
        maxNode = current;
        maxPrev = NULL;
        for (temp = current->next, prev = current; temp != NULL; prev = temp, temp = temp->next) {
            int maxScoreAverage = (maxNode->score[0] + maxNode->score[1] + maxNode->score[2]) / 3;
            int tempScoreAverage = (temp->score[0] + temp->score[1] + temp->score[2]) / 3;
            if (tempScoreAverage > maxScoreAverage) {
                maxNode = temp;
                maxPrev = prev;
            }
        }
        if (maxNode != current) {
            if (maxPrev != NULL) maxPrev->next = current;
            temp = maxNode->next;
            maxNode->next = current->next;
            current->next = temp;

            if (current == head) {
                head = maxNode;
            } else {
                for (temp = head; temp->next != current; temp = temp->next);
                temp->next = maxNode;
            }

            temp = current;
            current = maxNode;
            maxNode = temp;
        }
    }
}
//ͷ�巨����ڵ�
void InsertAtHead(STU*& head, STU* newNode) {
    newNode->next = head;
    head = newNode;
}
//β�巨����ڵ�
void InsertAtTail(STU*& head, STU* newNode) {
    if (head == NULL) {
        head = newNode;
    } else {
        STU* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    newNode->next = NULL;
}
//ͷɾ��
void DeleteAtHead(STU*& head) {
    if (head == NULL) {
        printf("����Ϊ�գ��޷�ɾ����\n");
    } else {
        STU* temp = head;
        head = head->next;
        free(temp);
    }
}
//βɾ��
void DeleteAtTail(STU*& head) {
    if (head == NULL) {
        printf("����Ϊ�գ��޷�ɾ����\n");
    } else if (head->next == NULL) {
        free(head);
        head = NULL;
    } else {
        STU* current = head;
        while (current->next->next != NULL) {
            current = current->next;
        }
        free(current->next);
        current->next = NULL;
    }
}
//����רҵ��༶�鿴ѧ����Ϣ
void ViewByMajorOrClass(STU* head, const char* major, int classNo) {
    STU* current = head;
    while (current != NULL) {
        if ((major == NULL || strcmp(current->major, major) == 0) && (classNo == 0 || current->classNo == classNo)) {
            OutOneNode(current);
        }
        current = current->next;
    }
}
//����רҵ��༶ɾ��ѧ����Ϣ
void DeleteByMajorOrClass(STU*& head, const char* major, int classNo) {
    STU *current = head, *prev = NULL;
    while (current != NULL) {
        if ((major == NULL || strcmp(current->major, major) == 0) && (classNo == 0 || current->classNo == classNo)) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            STU* temp = current;
            current = current->next;
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}
// ��ʾѧ������ϵͳ�����˵����������û�ѡ��
void menu() {
    int choice, n, index;
    printf("��Ĭ�ϴ�\"studentInit.dat\"�ļ�\n");
    Input("studentInit.dat",head);
    SortByID(head);
    while (1) {
        printf("---------------ѧ������ϵͳ�˵�---------------\n");
        printf("1.���ļ���ȡѧ����Ϣ\n");
        printf("2.��ʾ����ѧ����Ϣ\n");
        printf("3.ͨ��ѧ�Ż��������Ҳ��޸�ѧ����Ϣ\n");
        printf("4.�����ܷ���ߵ�ѧ��\n");
        printf("5.��ѧ����Ϣ��ƽ��������\n");
        printf("6.������ѧ����Ϣ\n");
        printf("7.ɾ��ָ��ѧ�Ż�������ѧ��\n");
        printf("8.����ָ��רҵ��ĳ�ſγ̳ɼ�С��ĳ��������ѧ��\n");
        printf("9.ɾ��ָ���༶��ĳ�ſγ̳ɼ�С��ĳ��������ѧ��\n");
        printf("10.�����º��ѧ����Ϣ���浽��һ���ļ���\n");
        printf("11.�˳�ϵͳ\n");
        printf("----------------------------------------------\n");
        if(head != NULL)printf("���ڴ򿪵���\"%s\"�ļ�\n",NowFileName);
        else printf("δ��ѧ����Ϣ�ļ�\n");
        printf("��ǰ�Ѿ�¼��%d��ѧ������Ϣ.\n", countStudents(head));
        printf("ѡ��һ��ѡ��: ");
        int read = scanf("%d", &choice);
        if (read != 1) {
            printf("�����������������\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                while (head != NULL) {
                    STU* temp = head;
                    head = head->next;
                    free(temp);
                }

                char filename[255];
                printf("�������ļ�����: ");
                scanf("%s", filename);
                getchar();
                head = NULL;
                Input(filename, head);
                printf("�����Ѵ� %s ������ɡ�\n", filename);
                strcpy(NowFileName, filename);
                SortByID(head);
                printf("����ѧ����ϢΪ:\n");
                Output(head);
            }
            system("pause");
            system("cls");
            break;
            case 2: {
                Output(head);
            }
            system("pause");
            system("cls");
            break;

            case 3: {
                char se[15];
                printf("�������������(1.ѧ�� 2.����):");
                int type;
                int read = scanf("%d",&type);
                if(read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                if(type != 1 && type != 2) {
                    printf("�����������������\n");
                    continue;
                }
                if(type == 1) {
                    printf("������Ҫ���ҵ�ѧ��ѧ��: ");
                    read = scanf("%s", se);
                    getchar();
                    if(read != 1) {
                        printf("�����������������\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    STU* found = Search_num(head, se);
                    if (found != NULL) {
                        Outputtitle();
                        OutOneNode(found);
                        printf("�Ƿ���ĸ�ѧ����Ϣ(y / n):");
                        char c;
                        int read = scanf("%c",&c);
                        if(read != 1) {
                            printf("�����������������\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        if(c == 'y') {
                            ChangeStudentInfo(found);
                        };
                    } else {
                        printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����\n", se);
                    }
                }else {
                    printf("������Ҫ���ҵ�ѧ������: ");
                    read = scanf("%s", se);
                    getchar();
                    if(read != 1) {
                        printf("�����������������\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    STU* found = Search_name(head, se);
                    if (found != NULL) {
                        Outputtitle();
                        OutOneNode(found);
                        printf("�Ƿ���ĸ�ѧ����Ϣ(y / n):");
                        char c;
                        int read = scanf("%c",&c);
                        if(read != 1) {
                            printf("�����������������\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        if(c == 'y') {
                            ChangeStudentInfo(found);
                        };
                    } else {
                        printf("δ�ҵ�����Ϊ %s ��ѧ����\n", se);
                    }
                }
            }
            system("pause");
            system("cls");
            break;

            case 4: {
                STU* maxScoreStudents = Max(head);
                if (maxScoreStudents != NULL) {
                    printf("�ܷ���ߵ�ѧ����Ϣ����:\n");
                    STU* current = maxScoreStudents;
                    while (current != NULL) {
                        Outputtitle();
                        OutOneNode(current);
                        current = current->next;
                    }

                    while (maxScoreStudents != NULL) {
                        STU* temp = maxScoreStudents;
                        maxScoreStudents = maxScoreStudents->next;
                        free(temp);
                    }
                } else {
                    printf("δ�ҵ�ѧ����Ϣ��\n");
                }
            }
            system("pause");
            system("cls");
            break;

           case 5: {
                SortByAverageScore(head);
                printf("ѧ����Ϣ�Ѱ�ƽ��������\n");
                Output(head);
               printf("�Ƿ����Ϊ���ļ�(y / n):");
                char c;
                int read = scanf("%c",&c);
                if(read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                if(c == 'y'){
                    char filename[255];
                    printf("�������ļ�����: ");
                    scanf("%s", filename);
                    Save(head,filename);
                }else if(c == 'n'){
                    printf("δ����\n");
                }else {
                    printf("�Ƿ�����");
                }
            }
            system("pause");
            system("cls");
            break;


            case 6: {
                STU newStudent;
                printf("������ѧ����Ϣ: \n");
                printf("ѧ��: ");
                scanf("%s", newStudent.ID);
                getchar();
                printf("����: ");
                scanf("%s", newStudent.name);
                getchar();
                printf("רҵ: ");
                scanf("%s", newStudent.major);
                getchar();
                printf("�༶: ");
                scanf("%d", &newStudent.classNo);
                getchar();
                printf("�γ�1�ɼ�: ");
                scanf("%d", &newStudent.score[0]);
                getchar();
                printf("�γ�2�ɼ�: ");
                scanf("%d", &newStudent.score[1]);
                getchar();
                printf("�γ�3�ɼ�: ");
                scanf("%d", &newStudent.score[2]);
                getchar();

                InsertList(head, newStudent);
                printf("��ѧ����Ϣ�Ѳ��롣\n");
            }
            system("pause");
            system("cls");
            break;

            case 7: {
                printf("�������������(1.ѧ�� 2.����):");
                int type;
                int read = scanf("%d",&type);
                if(read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                if(type != 1 && type != 2) {
                    printf("�����������������\n");
                    continue;
                }
                char se[15];
                if(type == 1) {
                    printf("������Ҫɾ����ѧ��ѧ��: ");
                    int read = scanf("%s", se);
                    getchar();
                    if (read != 1) {
                        printf("�����������������\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    Delete_num(head, se);
                }else {
                    printf("������Ҫɾ����ѧ������: ");
                    int read = scanf("%s", se);
                    getchar();
                    if (read != 1) {
                        printf("�����������������\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    Delete_name(head, se);
                }

            }
            system("pause");
            system("cls");
            break;
            case 8: {
                char major[15];
                int subject, grade;
                printf("������Ҫ���ҵ�ѧ����רҵ: ");
                int read = scanf("%s", major);
                getchar();
                if (read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                printf("������Ҫ���ҵĿγ�(1.�γ�1 2.�γ�2 3.�γ�3): ");
                read = scanf("%d", &subject);
                getchar();
                if (read != 1 || subject < 1 || subject > 3) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                printf("������ɼ�С�ڵķ���: ");
                read = scanf("%d", &grade);
                getchar();

                if (read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                STU* newList = Search_major_subject_score(head, major, subject - 1, grade);
                printf("�ҵ���ѧ����Ϣ����:\n");
                Output(newList);
                printf("����������ļ���:");
                char filename[255];
                scanf("%s",filename);
                Save(newList,filename);
                while (newList != NULL) {
                    STU* temp = newList;
                    newList = newList->next;
                    free(temp);
                }
            }
            system("pause");
            system("cls");
            break;
            case 9: {
                int classno, subject, grade;
                printf("������Ҫ���ҵ�ѧ���İ༶: ");
                int read = scanf("%d", &classno);
                getchar();
                if (read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                printf("������Ҫ���ҵĿγ�(1.�γ�1 2.�γ�2 3.�γ�3): ");
                read = scanf("%d", &subject);
                getchar();
                if (read != 1 || subject < 1 || subject > 3) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                printf("������ɼ�С�ڵķ���: ");
                read = scanf("%d", &grade);
                getchar();

                if (read != 1) {
                    printf("�����������������\n");
                    while (getchar() != '\n');
                    continue;
                }
                Delete_class_subject_score(head, classno, subject - 1, grade);
            }
            system("pause");
            system("cls");
            break;
            case 10: {
                char filename[255];
                printf("�������ļ�����: ");
                scanf("%s", filename);
                Save(head,filename);
            }
            system("pause");
            system("cls");
            break;
            case 11:
                printf("�˳�ϵͳ\n");
            return;

            default:
                printf("��Чѡ��\n");

        }
    }
}
int main() {
    menu();
    return 0;
}