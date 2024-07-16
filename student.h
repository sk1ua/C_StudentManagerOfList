//
// Created by 15857 on 24-7-3.
//

#ifndef STUDENT_H
#define STUDENT_H
typedef struct student{
    char ID[15]; //学号
    char name[15]; //姓名
    char major[10]; //专业
    int classNo; //班级
    int score[3]; //三门课成绩
    struct student* next; // 指向下一个节点的指针
} STU;

#endif //STUDENT_H
