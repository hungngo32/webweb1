#include "Menu.h"

bool checkAcademicYearAndSemester(string academicYear, string semester)
{
	string filePath = PATH_DATA;
	string* listAcademicYears = nullptr, * listSemesters = nullptr;
	int countAcademicYear = 0;

	filePath += "SchoolYear.txt";

	if (loadAcademicYearsAndSemester(filePath, listAcademicYears, listSemesters, countAcademicYear))
	{
		for (int i = 0; i < countAcademicYear; i++)
			if (isEqualAcademicYears(&academicYear, &listAcademicYears[i]))
				for (int j = 0; j < countAcademicYear; j++)
				{
					int nSemester = 0;
					string* semesters = StringToArrayString(listSemesters[j], ',', nSemester);

					for (int k = 0; k < nSemester; k++)
						if (semester == semesters[k])
						{
							delete[] semesters;
							return true;
						}

					delete[] semesters;
				}

		delete[] listSemesters;
		delete[] listAcademicYears;
	}

	return false;
}

bool checkStaff(Staff& s)
{
	string staffFilePath;
	Staff* staffList = nullptr;
	int countStaff;
	bool flag = false;

	staffFilePath = PATH_DATA;
	staffFilePath += "Staff.txt";
	if (loadListStaffs(staffFilePath, staffList, countStaff))
	{
		for (int i = 0; i < countStaff; i++)
		{
			if (staffList[i].info.acc.username == s.info.acc.username)
				if (staffList[i].info.acc.password == s.info.acc.password)
				{
					s = staffList[i];
					flag = true;
					break;
				}
		}

		delete[] staffList;
	}

	return flag;
}

bool checkLecturer(Lecturer& lec)
{
	string lectFilePath;
	Lecturer* lecturerList = nullptr;
	int countLect = 0;
	bool flag = false;

	lectFilePath = PATH_DATA;
	lectFilePath += "Lecturer.txt";
	if (loadListLecturers(lectFilePath, lecturerList, countLect))
	{
		for (int i = 0; i < countLect; i++)
		{
			if (lecturerList[i].info.acc.username == lec.info.acc.username)
				if (lecturerList[i].info.acc.password == lec.info.acc.password)
				{
					lec = lecturerList[i];
					flag = true;
					break;
				}
		}

		delete[] lecturerList;
	}

	return flag;
}

bool checkStudentForLogin(Student& st)
{
	string classPath, * className, listClassPath = "";
	Student* studentList = nullptr;
	int countStudent = 0, number_of_classes;
	bool flag = false;

	listClassPath = PATH_DATA;
	listClassPath += "Class.txt";
	if (loadListClassName(listClassPath, className, number_of_classes))
	{
		for (int i = 0; i < number_of_classes; i++)
		{
			classPath = createClassDirectoryWithFileName(className[i]);

			if (loadStudentList(classPath, studentList, countStudent))
			{
				for (int j = 0; j < countStudent; j++)
				{
					if (studentList[j].id == st.info.acc.username)
						if (studentList[j].info.acc.password == st.info.acc.password)
						{
							st = studentList[j];
							flag = true;
							break;
						}
				}

				delete[] studentList;
			}

			if (flag)
				break;
		}

		delete[] className;
	}

	return flag;
}

void getInputAcademicYearAndSemester(string& academicYear, string& semester)
{
	bool flag;

	viewAcademicYearsAndSemester();

	do {
		flag = true;

		cout << "\nEnter academic year: ";
		getline(cin, academicYear);
		cout << "Enter semester: ";
		getline(cin, semester);

		if (!checkAcademicYearAndSemester(academicYear, semester))
		{
			flag = false;
			cout << "The academic year and semester are invalid or do not exist." << endl;
		}

	} while (!flag);
}

void* login(short& role)
{
	Account log;
	Staff s;
	Student st;
	Lecturer lec;
	void* info = nullptr;
	role = 0;

	cout << "Login. Please enter these information." << endl;
	cout << "Username: ";
	getline(cin, log.username);
	cout << "Password (8 <= length <= 40): ";
	log.password = getInputPassword();//sua 1

	s.info.acc = log;
	lec.info.acc = log;
	st.info.acc = log;

	if (checkStaff(s)) {
		role = 1;
		info = new Staff;
		*(Staff*)info = s;
	}
	else if (checkLecturer(lec)) {
		role = 2;
		info = new Lecturer;
		*(Lecturer*)info = lec;
	}
	else if (checkStudentForLogin(st)) {
		role = 3;
		info = new Student;
		*(Student*)info = st;
	}

	return info;
}

void changePassword(Account& acc)
{
	string newPassword[2], curPassword;

	cout << "Password length: 8 <= length <= 40" << endl;

	cout << "Enter current password: ";
	curPassword = getInputPassword();

	cout << "Enter new password: ";
	newPassword[0] = getInputPassword();

	cout << "Enter new password again: ";
	newPassword[1] = getInputPassword();

	if (acc.password == curPassword) {
		if (newPassword[0] == newPassword[1]) {
			acc.password = newPassword[0];
			cout << "==> Password was successfully changed." << endl;
		}
		else cout << "The two new passwords are not the same." << endl;
	}
	else cout << "The current password is incorrect." << endl;
}

void viewGeneralInfo(GeneralInformation info)
{
	cout << "\n- Full name: " << info.fullName << endl;
	cout << "- Gender: " << ((info.gender == MALE) ? ("Male") : ("Female")) << endl;
}

void viewStaffInfo(Staff s)
{
	viewGeneralInfo(s.info);
	cout << endl;
}

void viewStudentInfo(Student st)
{
	viewGeneralInfo(st.info);
	cout << "- Student ID: " << st.id << endl;
	cout << "- Date of birth: " << st.dateOfBirth << endl;
	cout << "- Status: " << ((st.status) ? ("Available") : ("Expelled")) << endl << endl;
}

void viewLecturerInfo(Lecturer lec)
{
	viewGeneralInfo(lec.info);
	cout << "- Degree: " << lec.degree << endl << endl;
}

void showMenu()
{
	short role;
	int choice;
	Staff s;
	Lecturer lec;
	Student st;
	void* info = nullptr;

	while (true)
	{
		system("cls");
		cout << "|------------------------|" << endl;
		cout << "|        MAIN MENU       |" << endl;
		cout << "|------------------------|" << endl;
		cout << "| No |      Option       |" << endl;
		cout << "|------------------------|" << endl;
		cout << "| 1  | Log in            |" << endl;
		cout << "|------------------------|" << endl;
		cout << "| 2  | Exit              |" << endl;
		cout << "|------------------------|" << endl;

		choice = getChoice(1, 2);

		switch (choice)
		{
		case 1:
			system("cls");
			cin.ignore();
			info = login(role);

			switch (role)
			{
			case 1:
				s = *(Staff*)info;
				delete info;
				info = nullptr;

				showMenuOfStaff(s);
				break;
			case 2:
				lec = *(Lecturer*)info;
				delete info;
				info = nullptr;

				showMenuOfLecturer(lec);
				break;
			case 3:
				st = *(Student*)info;
				delete info;
				info = nullptr;

				showMenuOfStudent(st);
				break;
			default:
				cout << "Can't find account" << endl;
				break;
			}

			break;
		case 2:
			system("pause");
			exit(0);
		}

		system("pause");
	}
}

void showSubMenuOfStaff(Staff& staff, const string& academicYear, const string& semester) {
	system("cls");
	int choice;

	while (true)
	{
		system("cls");
		cout << "|------------------------------|" << endl;
		cout << "|             STAFF            |" << endl;
		cout << "|------------------------------|" << endl;
		cout << "| No |         Option          |" << endl;
		cout << "|------------------------------|" << endl;
		cout << "| 1  | Course                  |" << endl;
		cout << "|------------------------------|" << endl;
		cout << "| 2  | Scoreboard              |" << endl;
		cout << "|------------------------------|" << endl;
		cout << "| 3  | Attendance list         |" << endl;
		cout << "|------------------------------|" << endl;
		cout << "| 4  | Return to previous menu |" << endl;
		cout << "|------------------------------|" << endl;

		choice = getChoice(1, 4);

		switch (choice)
		{
		case 1:
			showMenuOfCourse(academicYear, semester);
			break;
		case 2:
			showMenuOfScoreboard(academicYear, semester);
			break;
		case 3:
			showMenuOfAttendanceList(academicYear, semester);
			break;
		case 4:
			return;
		}
	}
}

void showMenuOfStaff(Staff& staff)
{
	system("cls");

	string academicYear, semester, filePath = PATH_DATA;
	Staff* listStaffs = nullptr;
	int choice, countStaff = 0, idx;

	filePath += "Staff.txt";

	while (true)
	{
		system("cls");
		cout << "|------------------------------------------------|" << endl;
		cout << "|                     STAFF                      |" << endl;
		cout << "|------------------------------------------------|" << endl;
		cout << "| No |      Option                               |" << endl;
		cout << "|------------------------------------------------|" << endl;
		cout << "| 1  | Class                                     |" << endl;
		cout << "|------------------------------------------------|" << endl;
		cout << "| 2  | Create academic year and semester         |" << endl;
		cout << "|------------------------------------------------|" << endl;
		cout << "| 3  | Enter existing academic year and semester |" << endl;
		cout << "|------------------------------------------------|" << endl;
		cout << "| 4  | View profile info                         |" << endl;
		cout << "|------------------------------------------------|" << endl;
		cout << "| 5  | Change password                           |" << endl;
		cout << "|------------------------------------------------|" << endl;
		cout << "| 6  | Log out                                   |" << endl;
		cout << "|------------------------------------------------|" << endl;

		choice = getChoice(1, 6);

		system("cls");

		switch (choice)
		{
		case 1:
			showMenuOfClass();
			break;
		case 2:
			cin.ignore();
			createAcademicYearsAndSemester();
			showSubMenuOfStaff(staff, academicYear, semester);
			break;
		case 3:
			cin.ignore();
			getInputAcademicYearAndSemester(academicYear, semester);
			showSubMenuOfStaff(staff, academicYear, semester);
			break;
		case 4:
			viewStaffInfo(staff);
			system("pause");
			break;
		case 5:
			cin.ignore();
			changePassword(staff.info.acc);

			if (loadListStaffs(filePath, listStaffs, countStaff)) {
				idx = findValue(listStaffs, countStaff, sizeof(Staff), &staff, isEqualStaff);
				listStaffs[idx].info.acc.password = staff.info.acc.password;

				saveListStaffs(filePath, listStaffs, countStaff);
				delete[] listStaffs;
				listStaffs = nullptr;
			}

			system("pause");
			break;
		case 6:
			showMenu();
			return;
		}
	}
}

void showMenuOfClass()
{
	int choice;

	system("cls");

	while (true)
	{
		cout << "|-------------------------------------------------|" << endl;
		cout << "|                      CLASS                      |" << endl;
		cout << "|-------------------------------------------------|" << endl;
		cout << "| No |                   Option                   |" << endl;
		cout << "|-------------------------------------------------|" << endl;
		cout << "| 1  | Import students of a class from a csv file |" << endl;
		cout << "|-------------------------------------------------|" << endl;
		cout << "| 2  | Add a new student to a class               |" << endl;
		cout << "|-------------------------------------------------|" << endl;
		cout << "| 3  | Edit an existing student                   |" << endl;//thua
		cout << "|-------------------------------------------------|" << endl;
		cout << "| 4  | Remove a student                           |" << endl;//thua
		cout << "|-------------------------------------------------|" << endl;
		cout << "| 5  | Change students from one class to another  |" << endl;//thua
		cout << "|-------------------------------------------------|" << endl;
		cout << "| 6  | View list of classes                       |" << endl;//thua
		cout << "|-------------------------------------------------|" << endl;
		cout << "| 7  | View list of students in a class           |" << endl;//thua
		cout << "|-------------------------------------------------|" << endl;
		cout << "| 8  | Return to previous menu                    |" << endl;
		cout << "|-------------------------------------------------|" << endl;

		choice = getChoice(1, 8);

		system("cls");

		switch (choice)
		{
		case 1:
			cin.ignore();
			importClass();
			break;
		case 2:
			addNewStudent();
			break;
		case 3:
			editStudent();
			break;
		case 4:
			removeStudent();
			break;
		case 5:
			changeStudentClass();
			break;
		case 6:
			viewClassList();
			break;
		case 7:
			viewStudentsOfClass();
			break;
		case 8:
			return;
		}

		system("pause");
		system("cls");
	}
}

	void showMenuOfCourse(const string& academicYear, const string& semester)
	{
		int choice;

		while (true)
		{
			system("cls");

			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "|                               COURSE                              |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| No |                          Option                              |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 1  | Create / delete / view academic years, and semesters         |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 2  | Import courses from a csv file                               |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 3  | Add a new course                                             |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 4  | Edit an existing course                                      |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 5  | Remove a course                                              |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 6  | Remove a specific student from a course                      |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 7  | Add a specific student to a course                           |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 8  | View list of courses in the current semester                 |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 9  | View list of students of a course                            |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 10 | View attendance list of a course                             |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 11 | Create / update / delete / view all lecturers                |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;
			cout << "| 12 | Return to previous menu                                      |" << endl;
			cout << "|-------------------------------------------------------------------|" << endl;

			choice = getChoice(1, 12);

			system("cls");

			switch (choice)
			{
			case 1:
				showFunctionsOfAcademicYearAndSemester();
				break;
			case 2:
				cin.ignore();
				importCourses(academicYear, semester);
				break;
			case 3:
				addNewCourse(academicYear, semester);
				break;
			case 4:
				editAnExistingCourse(academicYear, semester);
				break;
			case 5:
				removeCourse(academicYear, semester);
				break;
			case 6:
				removeStudentFromCourse(academicYear, semester);
				break;
			case 7:
				addStudentToCourse(academicYear, semester);
				break;
			case 8:
				viewListCourses(academicYear, semester);
				break;
			case 9:
				viewListStudentsOfCourse(academicYear, semester);
				break;
			case 10:
				viewAttendanceListOfCourse(academicYear, semester);
				break;
			case 11:
				showFunctionsOfLecturer();
				break;
			case 12:
				return;
			}

			if (choice != 1 && choice != 4 && choice != 11)
				system("pause");
		}
	}

	void showMenuOfScoreboard(const string& academicYear, const string& semester)
	{
		int choice;

		while (true)
		{
			system("cls");

			cout << "|-------------------------------------------------|" << endl;
			cout << "|                    SCOREBOARD                   |" << endl;
			cout << "|-------------------------------------------------|" << endl;
			cout << "| No |                  Option                    |" << endl;
			cout << "|-------------------------------------------------|" << endl;
			cout << "| 1  | Search and view the scoreboard of a course |" << endl;
			cout << "|-------------------------------------------------|" << endl;
			cout << "| 2  | Export a scoreboard to a csv file          |" << endl;
			cout << "|-------------------------------------------------|" << endl;
			cout << "| 3  | Return to previous menu                    |" << endl;
			cout << "|-------------------------------------------------|" << endl;

			choice = getChoice(1, 3);

			system("cls");

			switch (choice)
			{
			case 1:
				viewScoreboardOfCourse(academicYear, semester);
				break;
			case 2:
				ExportScoreboardToCsv(academicYear, semester);
				break;
			case 3:
				return;
			}

			system("pause");
		}
	}

	void showMenuOfAttendanceList(const string& academicYear, const string& semester)
	{
		int choice;

		while (true)
		{
			system("cls");

			cout << "|--------------------------------------------------|" << endl;
			cout << "|                  ATTENDANCE LIST                 |" << endl;
			cout << "|--------------------------------------------------|" << endl;
			cout << "| No |                    Option                   |" << endl;
			cout << "|--------------------------------------------------|" << endl;
			cout << "| 1  | Search and view attendance list of a course |" << endl;
			cout << "|--------------------------------------------------|" << endl;
			cout << "| 2  | Export a attendance list to a csv file      |" << endl;
			cout << "|--------------------------------------------------|" << endl;
			cout << "| 3  | Return to previous menu                     |" << endl;
			cout << "|--------------------------------------------------|" << endl;

			choice = getChoice(1, 3);

			system("cls");

			switch (choice)
			{
			case 1:
				viewAttendanceListOfCourse(academicYear, semester);
				break;
			case 2:
				ExportAttendanceListToCsv(academicYear, semester);
				break;
			case 3:
				return;
			}

			system("pause");
		}
	}

void showMenuOfLecturer(Lecturer& lec)
{
	string academicYear, semester, filePath = PATH_DATA;
	Lecturer* listLecturers = nullptr;
	int choice, countLecturer = 0;

	filePath += "Lecturer.txt";

	system("cls");
	getInputAcademicYearAndSemester(academicYear, semester);

	while (true)
	{
		system("cls");

		cout << "|----------------------------------------------------|" << endl;
		cout << "|                      LECTURER                      |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| No |                   Option                      |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| 1  | View list of courses in the current semester  |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| 2  | View list of students of a course             |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| 3  | View attendance list of a course              |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| 4  | Edit an attendance                            |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| 5  | Import scoreboard of a course from a csv file |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| 6  | Edit grade of a student                       |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| 7  | View a scoreboard                             |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| 8  | View profile info                             |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| 9  | Change password                               |" << endl;
		cout << "|----------------------------------------------------|" << endl;
		cout << "| 10 | Log out                                       |" << endl;
		cout << "|----------------------------------------------------|" << endl;

		choice = getChoice(1, 10);

		system("cls");

		switch (choice)
		{
		case 1:
			viewListCourses(academicYear, semester, lec);
			break;
		case 2:
			viewListStudentsOfCourse(academicYear, semester, lec);
			break;
		case 3:
			viewAttendanceListOfCourseForLecturer(academicYear, semester, lec);
			break;
		case 4:
			editAttendance(academicYear, semester, lec);
			break;
		case 5:
			importScoreboardsOfCourse(academicYear, semester, lec);
			break;
		case 6:
			editGradeStudent(academicYear, semester, lec);
			break;
		case 7:
			viewScoreboardOfCourse(academicYear, semester, lec);
			break;
		case 8:
			viewLecturerInfo(lec);
			break;
		case 9:
			cin.ignore();
			changePassword(lec.info.acc);

			if (loadListLecturers(filePath, listLecturers, countLecturer)) {
				int idx = findValue(listLecturers, countLecturer, sizeof(Lecturer), &lec, isEqualLecturer);
				listLecturers[idx].info.acc.password = lec.info.acc.password;

				saveListLecturers(filePath, listLecturers, countLecturer);
				delete[] listLecturers;
				listLecturers = nullptr;
			}

			break;
		case 10:
			showMenu();
			return;
		}

		if (choice != 4)
			system("pause");
	}
}

void showMenuOfStudent(Student& st)
{
	string academicYear, semester, filePath;
	Student* listStudents = nullptr;
	int choice, countStudent = 0;

	filePath = createClassDirectoryWithFileName(st.ClassName);

	system("cls");
	getInputAcademicYearAndSemester(academicYear, semester);

	while (true)
	{
		system("cls");

		cout << "|--------------------------------------|" << endl;
		cout << "|                STUDENT               |" << endl;
		cout << "|--------------------------------------|" << endl;
		cout << "| No |             Option              |" << endl;
		cout << "|--------------------------------------|" << endl;
		cout << "| 1  | Check-in                        |" << endl;
		cout << "|--------------------------------------|" << endl;
		cout << "| 2  | View check-in result            |" << endl;
		cout << "|--------------------------------------|" << endl;
		cout << "| 3  | View schedules                  |" << endl;
		cout << "|--------------------------------------|" << endl;
		cout << "| 4  | View his/her scores of a course |" << endl;
		cout << "|--------------------------------------|" << endl;
		cout << "| 5  | View profile info               |" << endl;
		cout << "|--------------------------------------|" << endl;
		cout << "| 6  | Change password                 |" << endl;
		cout << "|--------------------------------------|" << endl;
		cout << "| 7  | Log out                         |" << endl;
		cout << "|--------------------------------------|" << endl;

		choice = getChoice(1, 7);

		system("cls");

		switch (choice)
		{
		case 1:
			cin.ignore();
			checkIn(academicYear, semester, st);
			break;
		case 2:
			cin.ignore();
			viewCheckInResult(academicYear, semester, st);
			break;
		case 3:
			viewSchedule(academicYear, semester, st);
			break;
		case 4:
			viewScores(academicYear, semester, st);
			break;
		case 5:
			viewStudentInfo(st);
			break;
		case 6:
			cin.ignore();
			changePassword(st.info.acc);

			if (loadStudentList(filePath, listStudents, countStudent)) {
				int idx = findValue(listStudents, countStudent, sizeof(Student), &st, isEqualStudentId);
				listStudents[idx].info.acc.password = st.info.acc.password;

				saveStudentList(filePath, listStudents, countStudent);
				delete[] listStudents;
				listStudents = nullptr;
			}

			break;
		case 7:
			showMenu();
			return;
		}

		system("pause");
	}
}