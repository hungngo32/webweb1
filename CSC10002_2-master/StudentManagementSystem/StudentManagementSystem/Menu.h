#ifndef MENU_H_
#define MENU_H_

#include "Staff.h"

bool checkAcademicYearAndSemester(string academicYear, string semester);

//Check accounts for login.
//Return account's generall information if found.
//Return NULL if notfound
bool checkStaff(Staff& s);
bool checkLecturer(Lecturer& lec);
bool checkStudentForLogin(Student& st);

void getInputAcademicYearAndSemester(string& academicYear, string& semester);

//Take a defined-empty account type variable.
//User Enter username and pass.
//Return the number of the account type:
//0 = Fail/no role/ can't find account
//1 = staff
//2 = lecturer
//3 = student
void* login(short& role);

void changePassword(Account& acc);

//Show general info (including full name, gender)
void viewGeneralInfo(GeneralInformation info);

//Show profile info (each function show )
void viewStaffInfo(Staff s);
void viewLecturerInfo(Lecturer lec);
void viewStudentInfo(Student st);

//Get attendance's account and their role from login.
//Show menu of jobs they can do.
//Account type:
//0 = Fail/no role/ can't find account
//1 = staff
//2 = lecturer
//3 = student
void showMenu();

void showSubMenuOfStaff(Staff& staff, const string& academicYear, const string& semester);

void showMenuOfStaff(Staff& staff);

void showMenuOfClass();

void showMenuOfCourse(const string& academicYear, const string& semester);

void showMenuOfScoreboard(const string& academicYear, const string& semester);

void showMenuOfAttendanceList(const string& academicYear, const string& semester);

void showMenuOfLecturer(Lecturer& lec);

void showMenuOfStudent(Student& st);

#endif