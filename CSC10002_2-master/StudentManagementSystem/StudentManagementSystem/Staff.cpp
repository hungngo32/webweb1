#include "Staff.h"

bool isEqualAcademicYears(void* year1, void* year2)
{
	return isEqualString(year1, year2);
}

bool isEqualStaff(void* s1, void* s2) {
	Staff* val1 = (Staff*)s1;
	Staff* val2 = (Staff*)s2;

	return (val1->info.fullName == val2->info.fullName && val1->info.acc.username == val2->info.acc.username);
}

bool ascendingAcademicYear(void* year1, void* year2)
{
	return ascendingString(year1, year2);
}

bool ascendingSemester(void* semester1, void* semester2)
{
	return ascendingString(semester1, semester2);
}

bool addClassName(string ClassName)
{
	string filePath = PATH_DATA;
	string* listClassName = nullptr;
	int countClassName = 0;
	bool flag = true;

	filePath += "Class.txt";

	if (loadListClassName(filePath, listClassName, countClassName))
	{
		if (!checkElementInArray(listClassName, countClassName, sizeof(string), &ClassName, isEqualString))
		{
			listClassName = (string*)pushBackArray(listClassName, countClassName, sizeof(string),
				&ClassName, allocArrayString, copyString, releaseArrayString);

			flag = saveListClassName(filePath, listClassName, countClassName);
		}
		else flag = false;

		delete[] listClassName;
	}
	else flag = false;

	return flag;
}

void addNewStudent()
{
	string filePathStudent, ClassName;
	Student* listStudents = nullptr, newStudent;
	int countStudent(0);

	ClassName = getInputClassName();
	filePathStudent = createClassDirectoryWithFileName(ClassName);

	if (!loadStudentList(filePathStudent, listStudents, countStudent))
	{
		cout << "Can not open student file." << endl;
		return;
	}

	cin.ignore();
	cout << "Enter new student's information: " << endl;
	cout << "ID: ";
	getline(cin, newStudent.id);
	cout << "Full name: ";
	getline(cin, newStudent.info.fullName);
	cout << "Date of birth (yyyy-mm-dd): ";
	getline(cin, newStudent.dateOfBirth);
	cout << "Gender (Male or Female): ";
	newStudent.info.gender = getInputGender();

	newStudent.status = true;
	newStudent.ClassName = ClassName;
	createAccountStudent(newStudent);

	if (!checkStudent(newStudent)) {
		listStudents = (Student*)pushBackArray(listStudents, countStudent, sizeof(Student),
			&newStudent, allocListStudents, copyStudent, releaseListStudents);

		if (saveStudentList(filePathStudent, listStudents, countStudent))
			cout << "Add new student successfully." << endl;
		else
			cout << "Can not open student file." << endl;
	}
	else
		cout << "This student is already on the list." << endl;

	delete[] listStudents;
}

void addLecturer(Lecturer lec) {
	string fileLecturer = PATH_DATA;
	Lecturer* listLecturers = nullptr;
	int countLecturer = 0, idx;

	fileLecturer += "Lecturer.txt";
	if (loadListLecturers(fileLecturer, listLecturers, countLecturer))
	{
		idx = findValue(listLecturers, countLecturer, sizeof(Lecturer), &lec, isEqualLecturer);

		if (idx == NOT_FOUND)
		{
			if (lec.info.acc.password.empty())
				lec.info.acc.password = DEFAULT_PASSWORD;

			listLecturers = (Lecturer*)pushBackArray(listLecturers, countLecturer, sizeof(Lecturer), &lec, allocListLecturers,
				copyLecturer, releaseListlecturers);
			bool tmp = saveListLecturers(fileLecturer, listLecturers, countLecturer);
		}

		delete[] listLecturers;
	}
}

void addNewCourse(const string& academicYear, const string& semester)
{
	string filePath, ClassName, newCourseFilePath;
	Course NewCourse, * listCourses = nullptr;
	int countCourses = 0, idx;

	ClassName = getInputClassName();

	cin.ignore();
	cout << "Enter new course ID: ";
	NewCourse.courseId = getInputCourseID();

	cout << "Enter new course name: ";
	getline(cin, NewCourse.courseName);

	cout << "Enter new course start date (YYYY MM DD): ";
	cin >> NewCourse.startDate.year >> NewCourse.startDate.month >> NewCourse.startDate.day;

	cout << "Enter new course end date (YYYY MM DD): ";
	cin >> NewCourse.endDate.year >> NewCourse.endDate.month >> NewCourse.endDate.day;

	cout << "Enter new course day of week (1: SUNDAY, 2: MONDAY,..., 7: SATURDAY): ";
	cin >> NewCourse.dayOfWeek;

	cout << "Enter new course start time (HH MM): ";
	cin >> NewCourse.startTime.hour >> NewCourse.startTime.minute;

	cout << "Enter new course end time (HH MM): ";
	cin >> NewCourse.endTime.hour >> NewCourse.endTime.minute;

	cout << "Enter new course room: ";
	cin.ignore();
	getline(cin, NewCourse.room);

	NewCourse.ClassName = ClassName;
	NewCourse.lecturer = addNewLecturer();
	NewCourse.status = true;

	filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, "Schedule", "txt");
	newCourseFilePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, NewCourse.courseId, "txt");

	bool flag = loadListCourses(filePath, listCourses, countCourses);
	idx = findValue(listCourses, countCourses, sizeof(Course), &NewCourse, isEqualCourseId);

	if (idx == NOT_FOUND)
	{
		listCourses = (Course*)pushBackArray(listCourses, countCourses, sizeof(Course), &NewCourse,
			allocListCourses, copyCourse, releaseListCourses);

		if (EnrollClassToCourse(newCourseFilePath, NewCourse))
		{
			cout << "Add new course successfully!" << endl;
			saveListCourses(filePath, listCourses, countCourses);
		}
		else
			cout << "Add new course failed!" << endl;
	}
	else
		cout << "Course exist!" << endl;

	if (flag) 
		delete[] listCourses;
}

void addStudentToCourse(const string& academicYear, const string& semester)
{
	string filePath, ClassName;
	Course course, * listCourses;
	StudentCourseInformation* listStudentCourseInformation, newStudent;
	int countStudent = 0, idx, countCourse = 0;

	cout << "Choose a class and a course:" << endl;
	if (getInputCourseFromSemester(academicYear, semester, ClassName, course, listCourses, countCourse))
	{
		filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, course.courseId, "txt");

		if (loadStudentCourseInformationList(filePath, listStudentCourseInformation, countStudent))
		{
			system("cls");
			cout << "Choose a class and a student wants to add to the course: " << endl;

			string destFilePath, destClassName;
			Student* listStudents = nullptr;
			int destCountStudent = 0;

			destClassName = getInputClassName();
			destFilePath = createClassDirectoryWithFileName(destClassName);
			if (loadStudentList(destFilePath, listStudents, destCountStudent))
			{
				idx = getInputStudent(listStudents, destCountStudent);

				if (idx != NOT_FOUND)
				{
					newStudent.st = listStudents[idx];

					if (!checkElementInArray(listStudentCourseInformation, countStudent, sizeof(StudentCourseInformation), &newStudent, isEqualStudentIdFromCourse))
					{
						initScoreboard(newStudent.scoreList);
						initAttendanceList(newStudent.attendList, course);
						newStudent.status = true;

						listStudentCourseInformation = (StudentCourseInformation*)pushBackArray(listStudentCourseInformation, countStudent,
							sizeof(StudentCourseInformation), &newStudent, allocStudentCourseInformationList, copyStudentCourseInformation,
							releaseStudentCourseInformationList);

						if (saveStudentCourseInformationList(filePath, listStudentCourseInformation, countStudent))
							cout << "Add new student to course successfully." << endl;
						else
							cout << "Can not open Student Course Information file." << endl;
					}
					else
						cout << "Student exist!" << endl;
				}

				delete[] listStudents;
			}
			else
				cout << "Can not open class file." << endl;

			releaseStudentCourseInformation(listStudentCourseInformation, countStudent);
		}
	}
}

bool addAcademicYearsAndSemester(string& academicYears, string& semester)
{
	string* listAcademicYears = nullptr, * listSemesters = nullptr;
	string filePath = PATH_DATA;
	int countAcademicYears = 0;
	bool result = false;

	filePath += "SchoolYear.txt";
	if (loadAcademicYearsAndSemester(filePath, listAcademicYears, listSemesters, countAcademicYears))
	{
		int idx = findValue(listAcademicYears, countAcademicYears, sizeof(string), &academicYears,
			isEqualAcademicYears);

		if (idx == NOT_FOUND)
		{
			int count = countAcademicYears;

			listAcademicYears = (string*)pushBackArray(listAcademicYears, countAcademicYears, sizeof(string),
				&academicYears, allocArrayString, copyString, releaseArrayString);

			listSemesters = (string*)pushBackArray(listSemesters, count, sizeof(string), &semester,
				allocArrayString, copyString, releaseArrayString);

			result = true;
		}
		else if (listSemesters[idx].find(semester) == string::npos)
		{
			listSemesters[idx] += "," + semester;
			result = true;
		}

		saveAcademicYearsAndSemester(filePath, listAcademicYears, listSemesters, countAcademicYears);

		delete[] listSemesters;
		delete[] listAcademicYears;
	}

	return result;
}

void importClass()
{
	string ClassName, filePath;

	cout << "==> Enter class: ";
	getline(cin, ClassName);
	if (!addClassName(ClassName))
	{
		cout << "Add class name failed or this class already exists." << endl;
		return;
	}

	cout << "==> Path of the csv file: ";
	getline(cin, filePath);

	importStudentListFromCsv(filePath);
}

bool EnrollClassToCourse(const string& filePath, const Course& course)
{
	StudentCourseInformation* listInfo = nullptr;
	Student* listStudents = nullptr;
	string fileClass;
	int countStudent = 0;
	bool flag = false;

	fileClass = createClassDirectoryWithFileName(course.ClassName);

	if (loadStudentList(fileClass, listStudents, countStudent))
	{
		listInfo = new StudentCourseInformation[countStudent];

		for (int i = 0; i < countStudent; i++)
		{
			initAttendanceList(listInfo[i].attendList, course);
			initScoreboard(listInfo[i].scoreList);
			listInfo[i].st = listStudents[i];
			listInfo[i].status = true;
			listInfo[i].status = true;
		}

		if (saveStudentCourseInformationList(filePath, listInfo, countStudent))
			flag = true;

		releaseStudentCourseInformation(listInfo, countStudent);
	}

	delete[] listStudents;
	return flag;
}

void importListCoursesFromCsv(const string& filePath, const string& academicYear, const string& semester)
{
	Course* listCourses = nullptr;
	int countCourse = 0;

	if (loadListCoursesFromCsv(filePath, listCourses, countCourse))
	{
		string fileCourse, fileSchedule;
		int countCourseImportSuccessful = countCourse;

		for (int i = 0; i < countCourse; i++)
		{
			fileCourse = createCourseDirectoryWithFileName(academicYear, semester, listCourses[i].ClassName,
				listCourses[i].courseId, "txt");

			if (!EnrollClassToCourse(fileCourse, listCourses[i]))
				countCourseImportSuccessful--;

			addLecturer(listCourses[i].lecturer);
		}

		fileSchedule = createCourseDirectoryWithFileName(academicYear, semester, listCourses[0].ClassName, "Schedule", "txt");
		if (saveListCourses(fileSchedule, listCourses, countCourse))
			cout << "Import successful (" << countCourseImportSuccessful << " / " << countCourse << ")." << endl;
		else
			cout << "Import failed." << endl;

		delete[] listCourses;
	}
	else
		cout << "Can not open file." << endl;
}

void importCourses(const string& academicYear, const string& semester) {
	string filePath, ClassName;

	cout << "Enter class name: ";
	getline(cin, ClassName);
	cout << "==> Path of the csv file: ";
	getline(cin, filePath);

	importListCoursesFromCsv(filePath, academicYear, semester);
}

void createAcademicYearsAndSemester()
{
	string academicYears, semester;

	cout << "Enter academic years: ";
	getline(cin, academicYears);

	cout << "Enter semester: ";
	getline(cin, semester);

	if (addAcademicYearsAndSemester(academicYears, semester))
		cout << "Successful." << endl;
	else
		cout << "Failed (This school year already exists)." << endl;

	system("pause");
}

void viewClassList()
{
	string* listClassName = nullptr;
	string filePath = PATH_DATA;
	int countClassName = 0;

	filePath += "Class.txt";

	if (!loadListClassName(filePath, listClassName, countClassName))
	{
		cout << "Can not open class file." << endl;
		return;
	}

	showListClassName(listClassName, countClassName);
	delete[] listClassName;
}

void viewStudentsOfClass()
{
	string filePath = PATH_DATA, ClassName;
	Student* listStudents = nullptr;
	int countStudent = 0;

	ClassName = getInputClassName();
	filePath = createClassDirectoryWithFileName(ClassName);
	loadStudentList(filePath, listStudents, countStudent);
	viewStudentList(listStudents, countStudent);

	delete[] listStudents;
}

void viewListStudentsOfCourse(const string& academicYear, const string& semester)
{
	string ClassName, filePath;
	Course course, * listCourses = nullptr;
	StudentCourseInformation* listInfo = nullptr;
	int countStudent = 0, countCourse = 0;

	if (getInputCourseFromSemester(academicYear, semester, ClassName, course, listCourses, countCourse))
	{
		delete[] listCourses;

		filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, course.courseId, "txt");

		if (loadStudentCourseInformationList(filePath, listInfo, countStudent))
		{
			cout << "\nList of students (Course ID: " << course.courseId << ", Class: " << ClassName << "):\n" << endl;
			viewStudentListFromCourse(listInfo, countStudent);
			releaseStudentCourseInformation(listInfo, countStudent);
		}
		else
			cout << "Can not open course file." << endl;
	}
}

void viewListCourses(const string& academicYear, const string& semester)
{
	string ClassName, filePath;
	Course* listCourses = nullptr;
	int countCourse;

	ClassName = getInputClassName();
	filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, "Schedule", "txt");

	if (loadListCourses(filePath, listCourses, countCourse))
	{
		showListCourses(listCourses, countCourse, ClassName);
		delete[] listCourses;
	}
	else
		cout << "Can not open course file." << endl;
}

void viewScoreboardOfCourse(const string& academicYear, const string& semester)
{
	string ClassName, filePath;
	Course course, * listCourses = nullptr;
	StudentCourseInformation* listInfo = nullptr;
	int countStudent = 0, countCourse = 0;

	if (getInputCourseFromSemester(academicYear, semester, ClassName, course, listCourses, countCourse))
	{
		delete[] listCourses;

		filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, course.courseId, "txt");
		if (loadStudentCourseInformationList(filePath, listInfo, countStudent))
		{
			cout << "Scoreboard of class " << ClassName << " (Course ID: " << course.courseId << "): \n" << endl;
			showListScoreboardsOfCourse(listInfo, countStudent);
			releaseStudentCourseInformation(listInfo, countStudent);
		}
		else
			cout << "Can not open course file" << endl;
	}
}

void viewAttendanceListOfCourse(const string& academicYear, const string& semester) {
	string ClassName, filePath;
	Course course, * listCourses = nullptr;
	StudentCourseInformation* listInfo = nullptr;
	int countStudent = 0, countCourse = 0;

	if (getInputCourseFromSemester(academicYear, semester, ClassName, course, listCourses, countCourse))
	{
		delete[] listCourses;

		filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, course.courseId, "txt");
		if (loadStudentCourseInformationList(filePath, listInfo, countStudent))
		{
			cout << "Attendance list of class " << ClassName << " (Course ID: " << course.courseId << "): \n" << endl;
			showAttendaceListOfCourse(listInfo, countStudent);

			releaseStudentCourseInformation(listInfo, countStudent);
		}
		else
			cout << "Can not open course file" << endl;
	}
}

void viewAcademicYearsAndSemester() {
	string filePath(PATH_DATA);
	string* listAcademicYears = nullptr, * listSemesters = nullptr;
	int countAcademicYears = 0;

	filePath += "SchoolYear.txt";

	if (loadAcademicYearsAndSemester(filePath, listAcademicYears, listSemesters, countAcademicYears)) {
		cout << "|" << setfill('-') << setw(43) << "-" << "|" << endl;
		cout << setfill(' ');

		cout << "| " << setw(4) << left << "No" << " | " << setw(16) << left << "Academic Years" << " | "
			<< setw(15) << left << "Semesters" << " |" << endl;

		cout << "|" << setfill('-') << setw(43) << "-" << "|" << endl;
		cout << setfill(' ');

		for (int i = 0; i < countAcademicYears; i++) {
			cout << "| " << setw(4) << left << i + 1 << " | " << setw(16) << left << listAcademicYears[i]
				<< " | " << setw(15) << left << listSemesters[i] << " |" << endl;

			cout << "|" << setfill('-') << setw(43) << "-" << "|" << endl;
			cout << setfill(' ');
		}

		delete[] listSemesters;
		delete[] listAcademicYears;
	}
	else
		cout << "Can not open file." << endl;
}

void viewLecturer(const Lecturer& lec)
{

	cout << "| " << setw(20) << left << lec.info.fullName
		<< " | " << setw(10) << left << ((lec.info.gender == MALE) ? ("MALE") : ("FEMALE"))
		<< " | " << setw(10) << left << lec.degree << " | " << endl;
}

void viewLecturerList()
{
	string filePath = PATH_DATA;
	Lecturer* listLecturers;
	int countLecturer = 0;

	filePath += "Lecturer.txt";
	if (loadListLecturers(filePath, listLecturers, countLecturer))
	{
		cout << "|" << setfill('-') << setw(55) << "-" << "|" << endl;
		cout << setfill(' ');

		cout << "| " << setw(4) << left << "No"
			<< " | " << setw(20) << left << "Full name"
			<< " | " << setw(10) << left << "Gender"
			<< " | " << setw(10) << left << "Degree" << " | " << endl;

		cout << "|" << setfill('-') << setw(55) << "-" << "|" << endl;
		cout << setfill(' ');

		for (int i = 0; i < countLecturer; i++)
		{
			cout << "| " << setw(4) << left << i + 1 << " ";
			viewLecturer(listLecturers[i]);

			cout << "|" << setfill('-') << setw(55) << "-" << "|" << endl;
			cout << setfill(' ');
		}

		delete[]listLecturers;
	}
}

void showFunctionsOfLecturer()
{
	int choice;
	Lecturer newLec;

	while (true)
	{
		system("cls");

		cout << "|----------------------------------|" << endl;
		cout << "|             LECTURER             |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| No |            Option           |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| 1  | Create                      |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| 2  | Update (edit)               |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| 3  | Delete                      |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| 4  | View                        |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| 5  | Return to previous menu     |" << endl;
		cout << "|----------------------------------|" << endl;

		choice = getChoice(1, 5);

		system("cls");

		switch (choice)
		{
		case 1:
			cin.ignore();
			newLec = addNewLecturer();
			break;
		case 2:
			editLecturer();
			break;
		case 3:
			deleteLecturer();
			break;
		case 4:
			viewLecturerList();
			break;
		case 5:
			return;
		}

		if (choice != 2)
			system("pause");
	}
}

void showFunctionsOfAcademicYearAndSemester() {
	int choice;

	while (true)
	{
		system("cls");

		cout << "|----------------------------------|" << endl;
		cout << "|    ACADEMIC YEAR AND SEMESTER    |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| No |            Option           |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| 1  | Create                      |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| 2  | Delete                      |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| 3  | View                        |" << endl;
		cout << "|----------------------------------|" << endl;
		cout << "| 4  | Return to previous menu     |" << endl;
		cout << "|----------------------------------|" << endl;

		choice = getChoice(1, 5);

		system("cls");

		switch (choice)
		{
		case 1:
			cin.ignore();
			createAcademicYearsAndSemester();
			break;
		case 2:
			deleteAcademicYearsAndSemester();
			break;
		case 3:
			viewAcademicYearsAndSemester();
			system("pause");
			break;
		case 4:
			return;
		}
	}
}

bool loadAcademicYearsAndSemester(const string& filePath, string*& listAcademicYears, string*& listSemesters,
	int& countAcademicYears)
{
	ifstream fin(filePath);

	if (!fin.is_open())
		return false;

	fin >> countAcademicYears;
	fin.ignore();

	if (countAcademicYears != 0)
	{
		listAcademicYears = new string[countAcademicYears];
		listSemesters = new string[countAcademicYears];

		for (int i = 0; i < countAcademicYears; i++)
		{
			getline(fin, listAcademicYears[i]);
			getline(fin, listSemesters[i]);
		}
	}

	fin.close();
	return true;
}

bool saveAcademicYearsAndSemester(const string& filePath, string* listAcademicYears, string* listSemesters,
	const int& countAcademicYears)
{
	ofstream fout(filePath);

	if (!fout.is_open())
		return false;

	for (int i = 0; i < countAcademicYears - 1; i++)
		for (int j = i + 1; j < countAcademicYears; j++)
			if (listAcademicYears[i] > listAcademicYears[j])
			{
				swap(listAcademicYears[i], listAcademicYears[j]);
				swap(listSemesters[i], listSemesters[j]);
			}

	for (int i = 0; i < countAcademicYears; i++)
	{
		int n;

		string* temp = StringToArrayString(listSemesters[i], ',', n);
		sortArray(temp, n, sizeof(string), ascendingSemester);
		listSemesters[i] = ArrayStringToString(temp, n, ',');

		delete[] temp;
	}

	fout << countAcademicYears << endl;
	for (int i = 0; i < countAcademicYears; i++)
	{
		fout << listAcademicYears[i] << endl;
		fout << listSemesters[i] << endl;
	}

	fout.close();
	return true;
}

void ExportScoreboardToCsv(const string& academicYear, const string& semester)
{
	string ClassName, filePath, destFilePath(PATH_DATA);
	Course course, * listCourses = nullptr;
	StudentCourseInformation* listInfo = nullptr;
	int countStudent = 0, countCourse = 0;

	if (getInputCourseFromSemester(academicYear, semester, ClassName, course, listCourses, countCourse))
	{
		delete[] listCourses;

		filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, course.courseId, "txt");

		if (loadStudentCourseInformationList(filePath, listInfo, countStudent))
		{
			destFilePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName,
				course.courseId + "-Scoreboard", "csv");

			if (saveListScoreboardsToCsv(destFilePath, listInfo, countStudent))
				cout << "Export successfully" << endl;
			else
				cout << "Export failed" << endl;

			releaseStudentCourseInformation(listInfo, countStudent);
		}
		else
			cout << "Can not open course file" << endl;
	}
}

void ExportAttendanceListToCsv(const string& academicYear, const string& semester) {
	string ClassName, filePath, destFilePath(PATH_DATA);
	Course course, * listCourses = nullptr;
	StudentCourseInformation* listInfo = nullptr;
	int countStudent = 0, countCourse = 0;

	if (getInputCourseFromSemester(academicYear, semester, ClassName, course, listCourses, countCourse))
	{
		delete[] listCourses;

		filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, course.courseId, "txt");

		if (loadStudentCourseInformationList(filePath, listInfo, countStudent))
		{
			destFilePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName,
				course.courseId + "-AttendanceList", "csv");

			if (saveAttendanceListOfCourseToCsv(destFilePath, listInfo, countStudent))
				cout << "Export successfully" << endl;
			else
				cout << "Export failed" << endl;

			releaseStudentCourseInformation(listInfo, countStudent);
		}
		else
			cout << "Can not open course file" << endl;
	}
}

void removeStudent()
{
	string filePath, ClassName;
	Student* listStudents, st;
	int countStudent = 0, idx;

	ClassName = getInputClassName();
	filePath = createClassDirectoryWithFileName(ClassName);
	if (loadStudentList(filePath, listStudents, countStudent))
	{
		int idx = getInputStudent(listStudents, countStudent);

		if (idx != NOT_FOUND)
		{
			listStudents[idx].status = 0;
			cout << "This student has been deleted." << endl;
		}

		saveStudentList(filePath, listStudents, countStudent);
		delete[] listStudents;
	}
}

void removeCourse(const string& academicYear, const string& semester)
{
	string ClassName, filePath;
	Course* listCourses = nullptr;
	Course key;
	int countCourse, idx, isRemove;

	if (getInputCourseFromSemester(academicYear, semester, ClassName, key, listCourses, countCourse))
	{
		filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, "Schedule", "txt");
		idx = findValue(listCourses, countCourse, sizeof(Course), &key, isEqualCourseId);

		if (idx != NOT_FOUND)
		{
			cout << "==> Do you want to delete this course? (Yes: 1, No: 0)" << endl;
			isRemove = getChoice(0, 1);

			if (isRemove)
			{
				listCourses[idx].status = false;
				if (saveListCourses(filePath, listCourses, countCourse - 1))
					cout << "==> Remove successfully." << endl;
				else
					cout << "==> Remove failed." << endl;
			}
		}
		else
			cout << "This course was not found." << endl;

		delete[] listCourses;
	}
}

void removeStudentFromCourse(const string& academicYear, const string& semester)
{
	string filePath, ClassName, CourseName;
	Course course, * listCourses;
	StudentCourseInformation* listStudentCourseInformation;
	int countStudent = 0, idx, countCourse = 0;

	CourseName = getInputCourseFromSemester(academicYear, semester, ClassName, course, listCourses, countCourse);
	filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, course.courseId, "txt");
	if (loadStudentCourseInformationList(filePath, listStudentCourseInformation, countStudent))
	{
		viewStudentListFromStudentCourseInformationList(listStudentCourseInformation, countStudent);

		cout << "\nChose the student you want to remove (enter a number between 1 and " << countStudent
			<< "or 0 if no student are deleted): " << endl;

		idx = getChoice(0, countStudent) - 1;

		if (idx != NOT_FOUND)
		{
			listStudentCourseInformation[idx].status = 0;
			cout << "This student has been deleted." << endl;
			saveStudentCourseInformationList(filePath, listStudentCourseInformation, countStudent);
		}

		releaseStudentCourseInformation(listStudentCourseInformation, countStudent);
	}
}

bool deleteAcademicYears(string academicYears)
{
	string* listAcademicYears = nullptr, * listSemesters = nullptr;
	string filePath = PATH_DATA;
	int countAcademicYears = 0;
	bool result = false;

	filePath += "SchoolYear.txt";

	if (loadAcademicYearsAndSemester(filePath, listAcademicYears, listSemesters, countAcademicYears))
	{
		int idx = findValue(listAcademicYears, countAcademicYears, sizeof(string), &academicYears,
			isEqualAcademicYears);

		if (idx != NOT_FOUND)
		{
			int count = countAcademicYears;

			listAcademicYears = (string*)deleteElementAtPosition(listAcademicYears, countAcademicYears, sizeof(string),
				idx, allocArrayString, copyString, releaseArrayString);

			listSemesters = (string*)deleteElementAtPosition(listSemesters, count, sizeof(string),
				idx, allocArrayString, copyString, releaseArrayString);

			saveAcademicYearsAndSemester(filePath, listAcademicYears, listSemesters, countAcademicYears);
			result = true;
		}

		delete[] listSemesters;
		delete[] listAcademicYears;
	}

	return result;
}

bool deleteSemesterFromAcademicYears(string academicYears, string semester)
{
	string* listAcademicYears = nullptr, * listSemesters = nullptr;
	string filePath = PATH_DATA;
	int countAcademicYears = 0;
	bool result = false;

	filePath += "SchoolYear.txt";

	if (loadAcademicYearsAndSemester(filePath, listAcademicYears, listSemesters, countAcademicYears))
	{
		int idx, i, j, count;

		idx = findValue(listAcademicYears, countAcademicYears, sizeof(string), &academicYears,
			isEqualAcademicYears);

		if (idx != NOT_FOUND)
		{
			i = listSemesters[idx].find(semester);

			if (i != string::npos)
			{
				j = listSemesters[idx].find(',', i);
				count = (j == string::npos) ? (j) : (j - i + 1);

				listSemesters[idx].erase(i, count);
				if (listSemesters[idx].empty() == false && listSemesters[idx].back() == ',')
					listSemesters[idx].pop_back();

				if (listSemesters[idx].length() == 0)
				{
					int sz = countAcademicYears;

					listAcademicYears = (string*)deleteElementAtPosition(listAcademicYears, countAcademicYears,
						sizeof(string), idx, allocArrayString, copyString, releaseArrayString);

					listSemesters = (string*)deleteElementAtPosition(listSemesters, sz, sizeof(string),
						idx, allocArrayString, copyString, releaseArrayString);
				}

				saveAcademicYearsAndSemester(filePath, listAcademicYears, listSemesters, countAcademicYears);
				result = true;
			}
		}

		delete[] listSemesters;
		delete[] listAcademicYears;
	}

	return result;
}

void deleteAcademicYearsAndSemester()
{
	string academicYears, semester;
	int choice;

	system("cls");

	cout << "|------------------------------------------------------|" << endl;
	cout << "| No |                      Option                     |" << endl;
	cout << "|------------------------------------------------------|" << endl;
	cout << "| 1  | Delete a specific academic year                 |" << endl;
	cout << "|------------------------------------------------------|" << endl;
	cout << "| 2  | Delete a specific semester from a academic year |" << endl;
	cout << "|------------------------------------------------------|" << endl;
	cout << "| 3  | Return to previous menu                         |" << endl;
	cout << "|------------------------------------------------------|" << endl;

	choice = getChoice(1, 3);

	switch (choice)
	{
	case 1:
		viewAcademicYearsAndSemester();

		cout << "Enter academic years: ";
		cin.ignore();
		getline(cin, academicYears);

		if (deleteAcademicYears(academicYears))
			cout << "Delete successful." << endl;
		else
			cout << "Delete failed." << endl;

		break;
	case 2:
		viewAcademicYearsAndSemester();

		cin.ignore();
		cout << "Enter academic years: ";
		getline(cin, academicYears);

		cout << "Enter semester: ";
		getline(cin, semester);

		if (deleteSemesterFromAcademicYears(academicYears, semester))
			cout << "Delete successful." << endl;
		else
			cout << "Delete failed (not found or can not open file)." << endl;

		break;
	case 3:
		break;
	}
}

bool deleteLecturer()
{
	string filePath = PATH_DATA;
	Lecturer* listLecturers, lec;
	int countLecturer = 0, idx;
	bool result = false;

	filePath += "Lecturer.txt";

	if (loadListLecturers(filePath, listLecturers, countLecturer))
	{
		viewLecturerList();
		idx = getChoice(1, countLecturer) - 1;

		listLecturers = (Lecturer*)deleteElementAtPosition(listLecturers, countLecturer, sizeof(Lecturer),
			idx, allocListLecturers, copyLecturer, releaseListlecturers);

		saveListLecturers(filePath, listLecturers, countLecturer);
		cout << "Delete successful" << endl;
		result = true;

		delete[] listLecturers;
	}

	return result;
}

void editCourseID(Course& course, const string academicYear, const string semester)
{
	int check;

	cout << "\nCurrent course ID: " << course.courseId << endl;
	cout << "==> Do you want to edit this course's ID? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		string newID;

		cout << "\nEnter new course ID: ";
		cin.ignore();
		newID = getInputCourseID();

		string currentFilePath = createCourseDirectoryWithFileName(academicYear, semester, course.ClassName, course.courseId, "txt");
		string newFilePath = createCourseDirectoryWithFileName(academicYear, semester, course.ClassName, newID, "txt");
		StudentCourseInformation* listInfo = nullptr;
		int countStudent = 0;

		if (currentFilePath != newFilePath) {
			if (loadStudentCourseInformationList(currentFilePath, listInfo, countStudent))
			{
				if (saveStudentCourseInformationList(newFilePath, listInfo, countStudent))
					course.courseId = newID;
				else
					cout << "Can't open student file." << endl;
			}
			else
				cout << "Can't open student file." << endl;
		}
	}
}

void editCourseName(Course& course)
{
	int check;

	cout << "\nCurrent course name: " << course.courseName << endl;
	cout << "==> Do you want to edit this course's name? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		string newName;

		cout << "\nEnter new course name: ";
		cin.ignore();
		getline(cin, newName);

		course.courseName = newName;
	}
}

void editCourseLecturer(Course& course)
{
	int check;

	cout << "\nCurrent course lecturer: " << course.lecturer.info.fullName << endl;
	cout << "==> Do you want to edit this course's lecturer? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check) {
		cin.ignore();
		course.lecturer = addNewLecturer();
	}
}

void editDate(Course& course)
{
	int check;

	cout << "\nCurrent course start date: " << toString(course.startDate) << endl;
	cout << "Current course end date: " << toString(course.endDate) << endl;
	cout << "==> Do you want to edit this course's start date and end date? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		Date newStartDate, newEndDate;

		cout << "\nEnter new course start date (YYYY MM DD): " << endl;
		cin >> newStartDate.year >> newStartDate.month >> newStartDate.day;

		cout << "\nEnter new course end date (YYYY MM DD): " << endl;
		cin >> newEndDate.year >> newEndDate.month >> newEndDate.day;

		course.startDate = newStartDate;
		course.endDate = newEndDate;
	}
}

void editDayOfWeek(Course& course)
{
	int check;

	cout << "\nCurrent course day of week: " << course.dayOfWeek << endl;
	cout << "==> Do you want to edit this course's day of week? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		int newDayOfWeek;

		cout << "\nEnter new course day of week (1: SUNDAY, 2: MONDAY,..., 7: SATURDAY): ";
		newDayOfWeek = getChoice(1, 7);

		course.dayOfWeek = newDayOfWeek;
	}
}

void editTime(Course& course)
{
	int check;

	cout << "\nCurrent course start time: " << course.startTime.hour << ":" << course.startTime.minute << endl;
	cout << "Current course start time: " << course.endTime.hour << ":" << course.endTime.minute << endl;
	cout << "==> Do you want to edit this course's start time and end time? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		Time newStartTime, newEndTime;

		cout << "\nEnter new course start time (HH MM): " << endl;
		cin >> newStartTime.hour >> newStartTime.minute;

		cout << "\nEnter new course end time (HH MM): " << endl;
		cin >> newEndTime.hour >> newEndTime.minute;

		course.startTime = newStartTime;
		course.endTime = newEndTime;
	}
}

void editRoom(Course& course)
{
	int check;

	cout << "\nCurrent course room: " << course.room << endl;
	cout << "==> Do you want to edit this course's room? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		string newRoom;

		cout << "\nEnter new course room: ";
		cin.ignore();
		getline(cin, newRoom);

		course.room = newRoom;
	}
}

void editAnExistingCourse(const string& academicYear, const string& semester)
{
	string ClassName, filePath;
	Course* listCourses = nullptr;
	Course key;
	int countCourse, idx;

	if (getInputCourseFromSemester(academicYear, semester, ClassName, key, listCourses, countCourse))
	{
		filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, "Schedule", "txt");
		idx = findValue(listCourses, countCourse, sizeof(Course), &key, isEqualCourseId);

		if (idx != NOT_FOUND)
		{
			int choice;

			system("cls");

			do
			{
				cout << "|------------------------------|" << endl;
				cout << "|          EDIT COURSE         |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| No |        Option           |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 1  | Course ID               |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 2  | Course Name             |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 3  | Lecturer                |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 4  | Date                    |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 5  | Day of week             |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 6  | Time                    |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 7  | Room                    |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 8  | Return to previous menu |" << endl;
				cout << "|------------------------------|" << endl;

				choice = getChoice(1, 8);

				switch (choice)
				{
				case 1:
					editCourseID(listCourses[idx], academicYear, semester);
					break;
				case 2:
					editCourseName(listCourses[idx]);
					break;
				case 3:
					editCourseLecturer(listCourses[idx]);
					break;
				case 4:
					editDate(listCourses[idx]);
					break;
				case 5:
					editDayOfWeek(listCourses[idx]);
					break;
				case 6:
					editTime(listCourses[idx]);
					break;
				case 7:
					editRoom(listCourses[idx]);
					break;
				case 8:
					break;
				}

				if (choice != 8) {
					system("pause");
					system("cls");
				}
			} while (choice != 8);

			saveListCourses(filePath, listCourses, countCourse);
		}
		else
			cout << "This course was not found." << endl;

		delete[] listCourses;
	}
}

void editStudentID(Student& st)
{
	int check;

	cout << "\nCurrent student id: " << st.id << endl;
	cout << "==> Do you want to edit this student id? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		cout << "Enter student ID to edit: ";
		cin.ignore();
		getline(cin, st.id);
	}
}

void editStudentName(Student& st)
{
	int check;

	cout << "\nCurrent student full name: " << st.info.fullName << endl;
	cout << "==> Do you want to edit this student name? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		cout << "Enter student full name to edit: ";
		cin.ignore();
		getline(cin, st.info.fullName);
	}
}

void editStudentDOB(Student& st)
{
	int check;

	cout << "\nCurrent student DOB (Date of birth): " << st.dateOfBirth << endl;
	cout << "==> Do you want to edit this student DOB? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		cout << "Enter student date of birth (yyyy-mm-dd) to edit: ";
		cin.ignore();
		getline(cin, st.dateOfBirth);
	}
}

void editStudentGender(Student& st)
{
	int check;

	cout << "\nCurrent student gender: " << ((st.info.gender == MALE) ? ("Male") : ("Female")) << endl;
	cout << "==> Do you want to edit this student gender? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		cout << "Enter student gender (Male or Female) to edit: ";
		cin.ignore();
		st.info.gender = getInputGender();
	}
}

void editStudent()
{
	string filePath, ClassName;
	Student* listStudents, st;
	int countStudent = 0;

	ClassName = getInputClassName();
	filePath = createClassDirectoryWithFileName(ClassName);

	if (loadStudentList(filePath, listStudents, countStudent))
	{
		int idx = getInputStudent(listStudents, countStudent);

		if (idx != NOT_FOUND)
		{
			int choice;

			system("cls");

			do
			{
				cout << "|------------------------------|" << endl;
				cout << "|          EDIT STUDENT        |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| No |        Option           |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 1  | Student ID              |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 2  | Student Full Name       |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 3  | Date of birth           |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 4  | Gender                  |" << endl;
				cout << "|------------------------------|" << endl;
				cout << "| 5  | Return to previous menu |" << endl;
				cout << "|------------------------------|" << endl;

				choice = getChoice(1, 5);

				switch (choice)
				{
				case 1:
					editStudentID(listStudents[idx]);
					break;
				case 2:
					editStudentName(listStudents[idx]);
					break;
				case 3:
					editStudentDOB(listStudents[idx]);
					break;
				case 4:
					editStudentGender(listStudents[idx]);
					break;
				case 5:
					break;
				}

				system("pause");
				system("cls");
			} while (choice != 5);
		}

		saveStudentList(filePath, listStudents, countStudent);
		delete[] listStudents;
	}
	else
		cout << "Can not open class file." << endl;
}

void editLecturerName(Lecturer& lec)
{
	int check;

	cout << "\nCurrent lecturer full name: " << lec.info.fullName << endl;
	cout << "==> Do you want to edit this lecturer name? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		cout << "Enter lecturer full name to edit: ";
		cin.ignore();
		getline(cin, lec.info.fullName);
	}
}

void editLecturerGender(Lecturer& lec)
{
	int check;

	cout << "\nCurrent lecturer gender: " << ((lec.info.gender == MALE) ? ("Male") : ("Female")) << endl;
	cout << "==> Do you want to edit this lecturer gender? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		cout << "Enter lecturer gender (Male or Female) to edit: ";
		cin.ignore();
		lec.info.gender = getInputGender();
	}
}

void editLecturerDegree(Lecturer& lec)
{
	int check;

	cout << "\nCurrent lecturer degree: " << lec.degree << endl;
	cout << "==> Do you want to edit this lecturer name? (Yes: 1, No: 0)" << endl;
	check = getChoice(0, 1);

	if (check)
	{
		cout << "Enter lecturer degree to edit: ";
		cin.ignore();
		getline(cin, lec.degree);
	}
}

void editLecturer()
{
	string filePath = PATH_DATA;
	Lecturer* listLecturers, lec;
	int countLecturer = 0, idx;

	filePath += "Lecturer.txt";

	if (loadListLecturers(filePath, listLecturers, countLecturer))
	{
		viewLecturerList();
		idx = getChoice(1, countLecturer) - 1;

		int choice;

		do
		{
			system("cls");

			cout << "|------------------------------|" << endl;
			cout << "|          EDIT LECTURER       |" << endl;
			cout << "|------------------------------|" << endl;
			cout << "| No |        Option           |" << endl;
			cout << "|------------------------------|" << endl;
			cout << "| 1  | Full Name               |" << endl;
			cout << "|------------------------------|" << endl;
			cout << "| 2  | Gender                  |" << endl;
			cout << "|------------------------------|" << endl;
			cout << "| 3  | Degree                  |" << endl;
			cout << "|------------------------------|" << endl;
			cout << "| 4  | Return to previous menu |" << endl;
			cout << "|------------------------------|" << endl;

			choice = getChoice(1, 4);
			system("cls");

			switch (choice)
			{
			case 1:
				editLecturerName(listLecturers[idx]);
				break;
			case 2:
				editLecturerGender(listLecturers[idx]);
				break;
			case 3:
				editLecturerDegree(listLecturers[idx]);
				break;
			case 4:
				break;
			}

			if (choice != 4)
				system("pause");
		} while (choice != 4);

		saveListLecturers(filePath, listLecturers, countLecturer);
		delete[] listLecturers;
	}
	else
		cout << "Can not open lecturer file." << endl;
}

void updateClassForStudentFromCourse(const Student& st, const string& newClass)
{
	string* listAcademicYears = nullptr, * listSemesters = nullptr, filePath = PATH_DATA;
	int countAcademicYears = 0;
	StudentCourseInformation key;
	
	filePath += "SchoolYear.txt";
	key.st = st;

	if (loadAcademicYearsAndSemester(filePath, listAcademicYears, listSemesters, countAcademicYears))
	{
		for (int i = 0; i < countAcademicYears; i++)
		{
			int countSemesters = 0;
			string* semesters = StringToArrayString(listSemesters[i], ',', countSemesters);

			for (int j = 0; j < countSemesters; j++) 
			{
				int countSchedule = 0;
				Schedule* listSchedule = getScheduleOfStudent(listAcademicYears[i], semesters[j], st, countSchedule);
				
				for (int k = 0; k < countSchedule; k++)
				{
					StudentCourseInformation* listInfo = nullptr;
					int countStudent = 0;
					string courseFilePath = createCourseDirectoryWithFileName(listAcademicYears[i], semesters[j], st.ClassName, listSchedule[k].courseId, "txt");

					if (loadStudentCourseInformationList(courseFilePath, listInfo, countStudent))
					{
						int idx = findValue(listInfo, countStudent, sizeof(StudentCourseInformation), &key, isEqualStudentIdFromCourse);

						if (idx != NOT_FOUND && listInfo[idx].st.ClassName != newClass)
						{
							listInfo[idx].st.ClassName = newClass;
							saveStudentCourseInformationList(courseFilePath, listInfo, countStudent);
						}

						releaseStudentCourseInformationList(listInfo, countStudent);
					}
				}

				if (listSchedule != nullptr)
					delete[] listSchedule;
			}

			if (semesters != nullptr)
				delete[] semesters;
		}

		if (countAcademicYears != 0)
		{
			delete[] listSemesters;
			delete[] listAcademicYears;
		}
	}
}

void changeStudentClass()
{
	string filePath, ClassName;
	Student* listStudents = nullptr;
	int countStudent = 0, idx;

	ClassName = getInputClassName();
	filePath = createClassDirectoryWithFileName(ClassName);
	if (loadStudentList(filePath, listStudents, countStudent))
	{
		idx = getInputStudent(listStudents, countStudent);

		if (idx != NOT_FOUND)
		{
			string DestClass, DestFilePath;
			Student* DestListStudents = nullptr;
			int countDestStudent = 0;

			system("cls");
			DestClass = getInputClassName();
			DestFilePath = createClassDirectoryWithFileName(DestClass);

			if (listStudents[idx].ClassName != DestClass)
			{
				if (loadStudentList(DestFilePath, DestListStudents, countDestStudent))
				{
					updateClassForStudentFromCourse(listStudents[idx], DestClass);
					listStudents[idx].ClassName = DestClass;

					DestListStudents = (Student*)pushBackArray(DestListStudents, countDestStudent, sizeof(Student),
						&listStudents[idx], allocListStudents, copyStudent, releaseListStudents);

					saveStudentList(DestFilePath, DestListStudents, countDestStudent);

					listStudents[idx].status = 0;
					cout << "Change successful." << endl;
					delete[] DestListStudents;
				}
				else
					cout << "Can not open dest class file." << endl;

				saveStudentList(filePath, listStudents, countStudent);
			}
			else
				cout << "Student exist" << endl;
		}

		delete[] listStudents;
	}
	else
		cout << "Can not open class file." << endl;
}
//
void deleteAcademicYearsAndSemester()
{
	string academicYears, semester;
	int choice;

	system("cls");

	cout << "|------------------------------------------------------|" << endl;
	cout << "| No |                      Option                     |" << endl;
	cout << "|------------------------------------------------------|" << endl;
	cout << "| 1  | Delete a specific academic year                 |" << endl;
	cout << "|------------------------------------------------------|" << endl;
	cout << "| 2  | Delete a specific semester from a academic year |" << endl;
	cout << "|------------------------------------------------------|" << endl;
	cout << "| 3  | Return to previous menu                         |" << endl;
	cout << "|------------------------------------------------------|" << endl;

	choice = getChoice(1, 3);

	switch (choice)
	{
	case 1:
		viewAcademicYearsAndSemester();

		cout << "Enter academic years: ";
		cin.ignore();
		getline(cin, academicYears);

		if (deleteAcademicYears(academicYears))
			cout << "Delete successful." << endl;
		else
			cout << "Delete failed." << endl;

		break;
	case 2:
		viewAcademicYearsAndSemester();

		cin.ignore();
		cout << "Enter academic years: ";
		getline(cin, academicYears);

		cout << "Enter semester: ";
		getline(cin, semester);

		if (deleteSemesterFromAcademicYears(academicYears, semester))
			cout << "Delete successful." << endl;
		else
			cout << "Delete failed (not found or can not open file)." << endl;

		break;
	case 3:
		break;
	}
}
