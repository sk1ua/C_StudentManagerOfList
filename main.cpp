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
//将学生信息读入系统
void Input(const char* filename, STU*& head) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("无法打开文件");
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

    printf("成功读入 %d 个学生信息\n", count);

    fclose(file);
}

//将学生信息写入文件
void InputToFile(const char* filename, STU* head) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("无法打开文件");
        return;
    }

    STU* current = head;
    while (current != NULL) {
        if (fwrite(current, sizeof(STU), 1, file) != 1) {
            perror("写入文件时出错");
            break;
        }
        current = current->next;
    }

    fclose(file);
}

// 标准输出格式
void OutOneNode(STU *p) {
    if (p != NULL) {
        printf("| %-10s | %-20s | %-20s | %-6d | %-6d | %-6d | %-6d | %-6d |\n",
               p->ID, p->name, p->major, p->classNo, p->score[0], p->score[1], p->score[2],
               p->score[0] + p->score[1] + p->score[2]);
    } else {
        printf("该节点为空。\n");
    }
}

// 标准输出表头
void Outputtitle() {
    printf("|     ID     |         Name         |        Major         | Class  | Score1 | Score2 | Score3 | Total  |\n");
}
// 显示所有学生信息
void Output(STU *head) {
    Outputtitle();
    STU* current = head;
    while (current != NULL) {
        OutOneNode(current);
        current = current->next;
    }
}
// 按学号排序学生信息
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
// 统计学生数量
int countStudents(STU* head) {
    int count = 0;
    STU* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}
// 保存学生信息
void Save(STU*& head, int n) {
    for (int i = 0; i < n; i++) {
        STU* newNode = (STU*)malloc(sizeof(STU));
        if (newNode == NULL) {
            printf("内存分配失败\n");
            return;
        }

        printf("输入学生信息: \n");
        printf("学号: ");
        scanf("%s", newNode->ID);
        getchar();
        printf("姓名: ");
        scanf("%s", newNode->name);
        getchar();
        printf("专业: ");
        scanf("%s", newNode->major);
        getchar();
        printf("班级: ");
        scanf("%d", &newNode->classNo);
        getchar();
        printf("课程1成绩: ");
        scanf("%d", &newNode->score[0]);
        getchar();
        printf("课程2成绩: ");
        scanf("%d", &newNode->score[1]);
        getchar();
        printf("课程3成绩: ");
        scanf("%d", &newNode->score[2]);
        getchar();

        newNode->next = head;
        head = newNode;
    }

    // 写入文件
    InputToFile("studentInit.dat", head);
    printf("录入学生信息成功\n");

    SortByID(head);
}
// 查找总分最高的学生
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
                printf("内存分配失败\n");
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
// 通过学号查找学生
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
// 通过姓名查找学生
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
// 通过学号删除学生
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
            printf("学号为 %s 的学生信息已删除。\n", num);
            InputToFile(NowFileName, head); // 将更新后的链表写入文件
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("未找到学号为 %s 的学生。\n", num);
}
// 通过姓名删除学生
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
            printf("姓名为 %s 的学生信息已删除。\n", name);
            InputToFile(NowFileName, head); // 将更新后的链表写入文件
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("未找到姓名为 %s 的学生。\n", name);
}

// 修改学生信息
void ChangeStudentInfo(STU* found) {
    if (found != NULL) {
        Outputtitle();
        OutOneNode(found);
        printf("输入新的学生信息: \n");
        printf("学号: ");
        scanf("%s", found->ID);
        getchar();
        printf("姓名: ");
        scanf("%s", found->name);
        getchar();
        printf("专业: ");
        scanf("%s", found->major);
        getchar();
        printf("班级: ");
        scanf("%d", &found->classNo);
        getchar();
        printf("课程1成绩: ");
        scanf("%d", &found->score[0]);
        getchar();
        printf("课程2成绩: ");
        scanf("%d", &found->score[1]);
        getchar();
        printf("课程3成绩: ");
        scanf("%d", &found->score[2]);
        getchar();

        printf("学生信息已更新。\n");
        Outputtitle();
        OutOneNode(found);
    }
}
// 插入新学生信息
void InsertList(STU*& head, STU stu) {
    STU* newNode = (STU*)malloc(sizeof(STU));
    if (newNode == NULL) {
        printf("内存分配失败\n");
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
// 查找指定专业、某门课程成绩小于某个分数的学生
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
// 将学生信息保存到文件
void Save(STU *head,char* filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("无法打开文件");
        return;
    }
    while (head != NULL) {
        fwrite(head, sizeof(STU), 1, file);
        head = head->next;
    }
    fclose(file);
}
// 删除指定班级、某门课程成绩小于某个分数的学生
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
    printf("是否另存为新文件(y / n):");
    char c;
    int read = scanf("%c",&c);
    if(read != 1) {
        printf("输入错误，请重新输入\n");
        while (getchar() != '\n');
    }
    if(c == 'y'){
        char filename[255];
        printf("请输入文件名称: ");
        scanf("%s", filename);
        Save(head,filename);
    }else if(c == 'n'){
        printf("未保存\n");
    }else{
        printf("非法输入");
    }
}
// 按平均分排序学生信息
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
//头插法插入节点
void InsertAtHead(STU*& head, STU* newNode) {
    newNode->next = head;
    head = newNode;
}
//尾插法插入节点
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
//头删法
void DeleteAtHead(STU*& head) {
    if (head == NULL) {
        printf("链表为空，无法删除。\n");
    } else {
        STU* temp = head;
        head = head->next;
        free(temp);
    }
}
//尾删法
void DeleteAtTail(STU*& head) {
    if (head == NULL) {
        printf("链表为空，无法删除。\n");
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
//按照专业或班级查看学生信息
void ViewByMajorOrClass(STU* head, const char* major, int classNo) {
    STU* current = head;
    while (current != NULL) {
        if ((major == NULL || strcmp(current->major, major) == 0) && (classNo == 0 || current->classNo == classNo)) {
            OutOneNode(current);
        }
        current = current->next;
    }
}
//按照专业或班级删除学生信息
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
// 显示学生管理系统的主菜单，并处理用户选择
void menu() {
    int choice, n, index;
    printf("已默认打开\"studentInit.dat\"文件\n");
    Input("studentInit.dat",head);
    SortByID(head);
    while (1) {
        printf("---------------学生管理系统菜单---------------\n");
        printf("1.从文件读取学生信息\n");
        printf("2.显示所有学生信息\n");
        printf("3.通过学号或姓名查找并修改学生信息\n");
        printf("4.查找总分最高的学生\n");
        printf("5.对学生信息按平均分排序\n");
        printf("6.插入新学生信息\n");
        printf("7.删除指定学号或姓名的学生\n");
        printf("8.查找指定专业、某门课程成绩小于某个分数的学生\n");
        printf("9.删除指定班级、某门课程成绩小于某个分数的学生\n");
        printf("10.将更新后的学生信息保存到另一个文件中\n");
        printf("11.退出系统\n");
        printf("----------------------------------------------\n");
        if(head != NULL)printf("现在打开的是\"%s\"文件\n",NowFileName);
        else printf("未打开学生信息文件\n");
        printf("当前已经录入%d个学生的信息.\n", countStudents(head));
        printf("选择一个选项: ");
        int read = scanf("%d", &choice);
        if (read != 1) {
            printf("输入错误，请重新输入\n");
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
                printf("请输入文件名称: ");
                scanf("%s", filename);
                getchar();
                head = NULL;
                Input(filename, head);
                printf("数据已从 %s 加载完成。\n", filename);
                strcpy(NowFileName, filename);
                SortByID(head);
                printf("读入学生信息为:\n");
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
                printf("请输入查找类型(1.学号 2.姓名):");
                int type;
                int read = scanf("%d",&type);
                if(read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                if(type != 1 && type != 2) {
                    printf("输入错误，请重新输入\n");
                    continue;
                }
                if(type == 1) {
                    printf("请输入要查找的学生学号: ");
                    read = scanf("%s", se);
                    getchar();
                    if(read != 1) {
                        printf("输入错误，请重新输入\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    STU* found = Search_num(head, se);
                    if (found != NULL) {
                        Outputtitle();
                        OutOneNode(found);
                        printf("是否更改该学生信息(y / n):");
                        char c;
                        int read = scanf("%c",&c);
                        if(read != 1) {
                            printf("输入错误，请重新输入\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        if(c == 'y') {
                            ChangeStudentInfo(found);
                        };
                    } else {
                        printf("未找到学号为 %s 的学生。\n", se);
                    }
                }else {
                    printf("请输入要查找的学生姓名: ");
                    read = scanf("%s", se);
                    getchar();
                    if(read != 1) {
                        printf("输入错误，请重新输入\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    STU* found = Search_name(head, se);
                    if (found != NULL) {
                        Outputtitle();
                        OutOneNode(found);
                        printf("是否更改该学生信息(y / n):");
                        char c;
                        int read = scanf("%c",&c);
                        if(read != 1) {
                            printf("输入错误，请重新输入\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        if(c == 'y') {
                            ChangeStudentInfo(found);
                        };
                    } else {
                        printf("未找到姓名为 %s 的学生。\n", se);
                    }
                }
            }
            system("pause");
            system("cls");
            break;

            case 4: {
                STU* maxScoreStudents = Max(head);
                if (maxScoreStudents != NULL) {
                    printf("总分最高的学生信息如下:\n");
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
                    printf("未找到学生信息。\n");
                }
            }
            system("pause");
            system("cls");
            break;

           case 5: {
                SortByAverageScore(head);
                printf("学生信息已按平均分排序。\n");
                Output(head);
               printf("是否另存为新文件(y / n):");
                char c;
                int read = scanf("%c",&c);
                if(read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                if(c == 'y'){
                    char filename[255];
                    printf("请输入文件名称: ");
                    scanf("%s", filename);
                    Save(head,filename);
                }else if(c == 'n'){
                    printf("未保存\n");
                }else {
                    printf("非法输入");
                }
            }
            system("pause");
            system("cls");
            break;


            case 6: {
                STU newStudent;
                printf("输入新学生信息: \n");
                printf("学号: ");
                scanf("%s", newStudent.ID);
                getchar();
                printf("姓名: ");
                scanf("%s", newStudent.name);
                getchar();
                printf("专业: ");
                scanf("%s", newStudent.major);
                getchar();
                printf("班级: ");
                scanf("%d", &newStudent.classNo);
                getchar();
                printf("课程1成绩: ");
                scanf("%d", &newStudent.score[0]);
                getchar();
                printf("课程2成绩: ");
                scanf("%d", &newStudent.score[1]);
                getchar();
                printf("课程3成绩: ");
                scanf("%d", &newStudent.score[2]);
                getchar();

                InsertList(head, newStudent);
                printf("新学生信息已插入。\n");
            }
            system("pause");
            system("cls");
            break;

            case 7: {
                printf("请输入查找类型(1.学号 2.姓名):");
                int type;
                int read = scanf("%d",&type);
                if(read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                if(type != 1 && type != 2) {
                    printf("输入错误，请重新输入\n");
                    continue;
                }
                char se[15];
                if(type == 1) {
                    printf("请输入要删除的学生学号: ");
                    int read = scanf("%s", se);
                    getchar();
                    if (read != 1) {
                        printf("输入错误，请重新输入\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    Delete_num(head, se);
                }else {
                    printf("请输入要删除的学生姓名: ");
                    int read = scanf("%s", se);
                    getchar();
                    if (read != 1) {
                        printf("输入错误，请重新输入\n");
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
                printf("请输入要查找的学生的专业: ");
                int read = scanf("%s", major);
                getchar();
                if (read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                printf("请输入要查找的课程(1.课程1 2.课程2 3.课程3): ");
                read = scanf("%d", &subject);
                getchar();
                if (read != 1 || subject < 1 || subject > 3) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                printf("请输入成绩小于的分数: ");
                read = scanf("%d", &grade);
                getchar();

                if (read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                STU* newList = Search_major_subject_score(head, major, subject - 1, grade);
                printf("找到的学生信息如下:\n");
                Output(newList);
                printf("请输入另存文件名:");
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
                printf("请输入要查找的学生的班级: ");
                int read = scanf("%d", &classno);
                getchar();
                if (read != 1) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                printf("请输入要查找的课程(1.课程1 2.课程2 3.课程3): ");
                read = scanf("%d", &subject);
                getchar();
                if (read != 1 || subject < 1 || subject > 3) {
                    printf("输入错误，请重新输入\n");
                    while (getchar() != '\n');
                    continue;
                }
                printf("请输入成绩小于的分数: ");
                read = scanf("%d", &grade);
                getchar();

                if (read != 1) {
                    printf("输入错误，请重新输入\n");
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
                printf("请输入文件名称: ");
                scanf("%s", filename);
                Save(head,filename);
            }
            system("pause");
            system("cls");
            break;
            case 11:
                printf("退出系统\n");
            return;

            default:
                printf("无效选项\n");

        }
    }
}
int main() {
    menu();
    return 0;
}