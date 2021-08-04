#include "Course.h"

bool isEqualCourseId(void* val1, void* val2)
{
	Course* course1, * course2;

	course1 = (Course*)val1;
	course2 = (Course*)val2;

	return (course1->courseId == course2->courseId);
}

bool ascendingStudentIdOfCourse(void* val1, void* val2) {
	StudentCourseInformation* st1 = (StudentCourseInformation*)val1;
	StudentCourseInformation* st2 = (StudentCourseInformation*)val2;

	return ascendingString(&st1->st.id, &st2->st.id);
}

string createCourseDirectoryWithFileName(const string& academicYear, const string& semester,
	const string& ClassName, const string& argLast, const string& FileExtension)
{
	string result = PATH_DATA;
	result += academicYear + "-" + semester + "-" + ClassName + "-" + argLast + "." + FileExtension;

	return result;
}

bool getInputCourseFromSemester(const string& academicYear, const string& semester, string& ClassName,
	Course& course, Course*& listCourses, int& countCourse)
{
	string filePath;
	int choice = 0;

	ClassName = getInputClassName();
	filePath = createCourseDirectoryWithFileName(academicYear, semester, ClassName, "Schedule", "txt");

	if (!loadListCourses(filePath, listCourses, countCourse))
		return false;

	showListCourses(listCourses, countCourse, ClassName);
	choice = getChoice(1, countCourse);
	course = listCourses[choice - 1];

	return true;
}

void initAttendanceList(AttendanceList& listAttends, const Course& course)
{
	listAttends.countDate = calcNumberOfWeeks(course);
	listAttends.dateList = new Date[listAttends.countDate];
	listAttends.status = new bool[listAttends.countDate] {false};
	listAttends.startTime = course.startTime;
	listAttends.endTime = course.endTime;

	listAttends.dateList[0] = course.startDate;
	for (int i = 1; i < listAttends.countDate; i++)
		listAttends.dateList[i] = nextWeek(listAttends.dateList[i - 1]);
}

void initAttendanceList(AttendanceList& listAttends, const int& numberOfWeeks)
{
	listAttends.countDate = numberOfWeeks;
	listAttends.dateList = new Date[listAttends.countDate];
	listAttends.status = new bool[listAttends.countDate]{ false };
}

void initScoreboard(Scoreboard& score, const double& midterm, const double& _final,
	const double& bonus, const double& total)
{
	score.midterm = midterm;
	score._final = _final;
	score.bonus = bonus;
	score.total = total;
}

void copyAttendanceList(AttendanceList& dest, const AttendanceList& src)
{
	dest.countDate = src.countDate;
	dest.dateList = new Date[dest.countDate];
	dest.status = new bool[dest.countDate];
	dest.startTime = src.startTime;
	dest.endTime = src.endTime;

	for (int i = 0; i < dest.countDate; i++)
	{
		dest.dateList[i] = src.dateList[i];
		dest.status[i] = src.status[i];
	}
}

void* allocListCourses(const int& countCourse)
{
	Course* arr = new Course[countCourse];
	return arr;
}

void* allocStudentCourseInformationList(const int& countStudent)
{
	StudentCourseInformation* arr = new StudentCourseInformation[countStudent];
	return arr;
}

void copyCourse(void* lec1, void* lec2)
{
	*(Course*)lec1 = *(Course*)lec2;
}

void copyStudentCourseInformation(void* sci1, void* sci2)
{
	StudentCourseInformation* val1, * val2;
	
	val1 = (StudentCourseInformation*)sci1;
	val2 = (StudentCourseInformation*)sci2;

	val1->st = val2->st;
	val1->scoreList = val2->scoreList;
	val1->status = val2->status;
	copyAttendanceList(val1->attendList, val2->attendList);
}

void releaseListCourses(void* listCourses, const int& countCourse)
{
	if (listCourses)
	{
		Course* arr = (Course*)listCourses;
		delete[] arr;
	}
}

void releaseStudentCourseInformationList(void* listInfo, const int& countStudent)
{
	if (listInfo)
	{
		StudentCourseInformation* arr = (StudentCourseInformation*)listInfo;
		releaseStudentCourseInformation(arr, countStudent);
	}
}

void loadCourseFromCsv(ifstream& fin, Course& course)
{
	Lecturer& lec = course.lecturer;
	string gender, dayOfWeek;
	char ignore;

	getline(fin, course.courseId, ',');
	getline(fin, course.courseName, ',');
	getline(fin, course.ClassName, ',');

	getline(fin, lec.info.acc.username, ',');
	getline(fin, lec.info.fullName, ',');
	getline(fin, lec.degree, ',');
	getline(fin, gender, ',');

	fin >> course.startDate.day >> ignore >> course.startDate.month >> ignore >> course.startDate.year;
	fin >> ignore;

	fin >> course.endDate.day >> ignore >> course.endDate.month >> ignore >> course.endDate.year;
	fin >> ignore;

	getline(fin, dayOfWeek, ',');

	fin >> course.startTime.hour >> ignore >> course.startTime.minute >> ignore;
	fin >> course.endTime.hour >> ignore >> course.endTime.minute >> ignore;

	getline(fin, course.room);

	lec.info.gender = (gender == "Male") ? (Gender::MALE) : (Gender::FEMALE);
	course.dayOfWeek = convertWeekdayStringToNumber(dayOfWeek);
	course.status = true;
}

void loadCourse(ifstream& fin, Course& course)
{
	Lecturer& lec = course.lecturer;

	getline(fin, course.courseId);
	getline(fin, course.courseName);
	getline(fin, course.ClassName);

	getline(fin, lec.info.acc.username);
	getline(fin, lec.info.fullName);
	getline(fin, lec.degree);
	fin >> lec.info.gender;
	fin.ignore();

	fin >> course.startDate.year >> course.startDate.month >> course.startDate.day;
	fin >> course.endDate.year >> course.endDate.month >> course.endDate.day;
	fin >> course.dayOfWeek;
	fin.ignore();

	fin >> course.startTime.hour >> course.startTime.minute;
	fin >> course.endTime.hour >> course.endTime.minute;
	fin.ignore();

	getline(fin, course.room);
	fin >> course.status;
}

void saveCourse(ofstream& fout, Course& course)
{
	Lecturer& lec = course.lecturer;

	fout << course.courseId << endl << course.courseName << endl
		<< course.ClassName << endl;

	fout << lec.info.acc.username << endl << lec.info.fullName << endl
		<< lec.degree << endl << lec.info.gender << endl;

	fout << toString(course.startDate, ' ') << endl;
	fout << toString(course.endDate, ' ') << endl;
	fout << course.dayOfWeek << endl;
	fout << course.startTime.hour << " " << course.startTime.minute << endl;
	fout << course.endTime.hour << " " << course.endTime.minute << endl;

	fout << course.room << endl << course.status << endl;
}

void loadStudentFromTxt(ifstream& fin, Student& st)//done
{
	getline(fin, st.id, DELIMIT_TXT);
	getline(fin, st.info.acc.password, DELIMIT_TXT);
	getline(fin, st.info.fullName, DELIMIT_TXT);
	getline(fin, st.dateOfBirth, DELIMIT_TXT);
	getline(fin, st.ClassName, DELIMIT_TXT);
	fin >> st.info.gender >> st.status;

	st.info.acc.username = st.id;
}

void saveStudent(ofstream& fout, const Student& st)//vo student.cpp
{
	fout << st.id << endl << st.info.acc.password << endl;
	fout << st.info.fullName << endl << st.dateOfBirth << endl;
	fout << st.ClassName << endl << st.info.gender << endl << st.status << endl;
}

void loadScoreboard(ifstream& fin, Scoreboard& scoreboard, const string& FileExtension)
{
	bool flag = (FileExtension == "csv") ? (true) : (false);
	char delim;

	fin >> scoreboard.midterm;
	if (flag)
		fin >> delim;

	fin >> scoreboard._final;
	if (flag)
		fin >> delim;

	fin >> scoreboard.bonus;
	if (flag)
		fin >> delim;

	fin >> scoreboard.total;
}

void saveScoreboard(ofstream& fout, const Scoreboard& scoreboard)
{
	fout << scoreboard.midterm << endl
		<< scoreboard._final << endl
		<< scoreboard.bonus << endl
		<< scoreboard.total << endl;
}

void loadAttendanceList(ifstream& fin, AttendanceList& listAttends)
{
	for (int i = 0; i < listAttends.countDate; i++)
	{
		fin >> listAttends.dateList[i].year >> listAttends.dateList[i].month >> listAttends.dateList[i].day;
		fin >> listAttends.startTime.hour >> listAttends.startTime.minute;
		fin >> listAttends.endTime.hour >> listAttends.endTime.minute;
		fin >> listAttends.status[i];
	}
}

void saveAttendanceList(ofstream& fout, const AttendanceList& listAttends)
{
	for (int i = 0; i < listAttends.countDate; i++)
	{
		fout << toString(listAttends.dateList[i], ' ') << " ";
		fout << listAttends.startTime.hour << " " << listAttends.startTime.minute << " ";
		fout << listAttends.endTime.hour << " " << listAttends.endTime.minute << " ";
		fout << listAttends.status[i] << endl;
	}
}

bool loadListCoursesFromCsv(const string& filePath, Course*& listCourses, int& countCourse)
{
	ifstream fin(filePath);

	if (!fin.is_open())
		return false;

	string ignoreLine, filePathToSaveData;
	int temp;

	listCourses = new Course[MAX_SIZE];

	getline(fin, ignoreLine);
	while (!fin.eof())
	{
		fin >> temp;
		fin.ignore();

		loadCourseFromCsv(fin, listCourses[countCourse]);
		countCourse++;
	}

	fin.close();
	return true;
}

bool loadListCourses(const string& filePath, Course*& listCourses, int& countCourse)
{
	ifstream fin(filePath);

	if (!fin.is_open())
		return false;

	fin >> countCourse;
	fin.ignore();

	listCourses = new Course[countCourse];

	for (int i = 0; i < countCourse; i++)
	{
		loadCourse(fin, listCourses[i]);
		fin.ignore();
	}

	fin.close();
	return true;
}

bool saveListCourses(const string& filePath, Course* listCourses, const int& countCourse)
{
	ofstream fout(filePath);
	int idx = 0;

	if (!fout.is_open())
		return false;

	fout << countCourse << endl;
	for (int i = 0; i < countCourse; i++, idx++)
	{
		if (listCourses[idx].status)
			saveCourse(fout, listCourses[idx]);
		else
			i--;
	}

	fout.close();
	return true;
}

bool saveAttendanceListOfCourseToCsv(const string& filePath, StudentCourseInformation* listInfo, const int& countStudent) {
	ofstream fout(filePath);
	string status;

	if (!fout.is_open())
		return false;

	fout << "No,Student ID,Fullname";
	for (int i = 0; i < listInfo[0].attendList.countDate; i++)
		fout << "," << toString(listInfo[0].attendList.dateList[i]);
	fout << endl;

	for (int i = 0; i < countStudent; i++) {
		fout << i + 1 << "," << listInfo[i].st.id << "," << listInfo[i].st.info.fullName;

		for (int j = 0; j < listInfo[0].attendList.countDate; j++) {
			status = "";

			if (listInfo[i].attendList.status[j])
				status = "X";

			fout << "," << status;
		}

		fout << endl;
	}

	fout.close();
	return true;
}

bool loadStudentCourseInformationList(const string& filePath, StudentCourseInformation*& listInfo,
	int& countStudent)
{
	ifstream fin(filePath);
	int countWeek;

	if (!fin.is_open())
		return false;

	fin >> countStudent;
	fin >> countWeek;
	fin.ignore();

	listInfo = new StudentCourseInformation[countStudent];

	for (int i = 0; i < countStudent; i++)
	{
		initAttendanceList(listInfo[i].attendList, countWeek);

		getline(fin, listInfo[i].st.id);
		getline(fin, listInfo[i].st.info.fullName);
		getline(fin, listInfo[i].st.ClassName, DELIMIT_TXT);

		loadScoreboard(fin, listInfo[i].scoreList, "txt");
		loadAttendanceList(fin, listInfo[i].attendList);
		fin >> listInfo[i].status;
		fin.ignore();
	}

	fin.close();
	return true;
}

bool saveStudentCourseInformationList(const string& filePath, StudentCourseInformation* listInfo,
	const int& countStudent)
{
	ofstream fout(filePath);
	int  nStudent = 0;

	if (!fout.is_open())
		return false;

	for (int i = 0; i < countStudent; i++)
		if (listInfo[i].status)
			nStudent++;

	fout << nStudent << endl;
	fout << listInfo[0].attendList.countDate << endl;

	sortArray(listInfo, countStudent, sizeof(StudentCourseInformation), ascendingStudentIdOfCourse);

	for (int i = 0; i < countStudent; i++)
	{
		if (listInfo[i].status)
		{
			fout << listInfo[i].st.id << endl << listInfo[i].st.info.fullName << endl << listInfo[i].st.ClassName << endl;
			saveScoreboard(fout, listInfo[i].scoreList);
			saveAttendanceList(fout, listInfo[i].attendList);
			fout << listInfo[i].status << endl;
		}
	}

	fout.close();
	return true;
}

void viewStudentListFromStudentCourseInformationList(StudentCourseInformation* sci, int countStudent)
{
	cout << "|" << setfill('-') << setw(105) << "-" << "|" << endl;
	cout << setfill(' ');

	cout << "| " << setw(4) << left << "No"
		<< " | " << setw(12) << left << "Student ID" << " | " << setw(35) << left << "Full name"
		<< " | " << setw(10) << left << "Gender" << " | " << setw(15) << left << "Date of birth"
		<< " | " << setw(12) << left << "Class name" << " |" << endl;

	cout << "|" << setfill('-') << setw(105) << "-" << "|" << endl;
	cout << setfill(' ');

	for (int i = 0; i < countStudent; i++)
	{
		cout << "| " << setw(4) << left << i + 1 << " ";

		viewStudent(sci[i].st);

		cout << "|" << setfill('-') << setw(105) << "-" << "|" << endl;
		cout << setfill(' ');
	}
}

void showAttendaceListOfCourse(StudentCourseInformation*& listInfo, const int& countStudent) {
	int len = 59 + (15 * listInfo[0].attendList.countDate);
	string status;

	cout << "|" << setfill('-') << setw(len) << "-" << "|" << endl;
	cout << setfill(' ');

	cout << "| " << setw(4) << left << "No" << " | " << setw(12) << "Student ID"
		<< " | " << setw(35) << "Full name";

	for (int i = 0; i < listInfo[0].attendList.countDate; i++)
		cout << " | " << setw(12) << toString(listInfo[0].attendList.dateList[i]);
	cout << " |" << endl;

	cout << "|" << setfill('-') << setw(len) << "-" << "|" << endl;
	cout << setfill(' ');

	for (int i = 0; i < countStudent; i++)
	{
		cout << "| " << setw(4) << left << i + 1
			<< " | " << setw(12) << left << listInfo[i].st.id
			<< " | " << setw(35) << left << listInfo[i].st.info.fullName;

		for (int j = 0; j < listInfo[0].attendList.countDate; j++) {
			status = "";

			if (listInfo[i].attendList.status[j])
				status = "X";

			cout << " | " << setw(6) << right << status << setw(6) << "";
		}

		cout << " |" << endl;
		cout << "|" << setfill('-') << setw(len) << "-" << "|" << endl;
		cout << setfill(' ');
	}
}

void showListScoreboardsOfCourse(StudentCourseInformation*& listInfo, const int& countStudent)
{
	cout << "|" << setfill('-') << setw(112) << "-" << "|" << endl;
	cout << setfill(' ');

	cout << "| " << setw(5) << left << " No"
		<< " | " << setw(12) << left << "Student ID" << " | " << setw(35) << left << "Full name"
		<< " | " << setw(10) << left << "  Midterm" << " | " << setw(10) << left << "  Final"
		<< " | " << setw(10) << left << "  Bonus" << " | " << setw(10) << left << "  Total" << " |" << endl;

	cout << "|" << setfill('-') << setw(112) << "-" << "|" << endl;
	cout << setfill(' ');

	for (int i = 0; i < countStudent; i++)
	{
		cout << "| " << setw(3) << right << i + 1 << "  ";
		viewScoreboardOfStudent(listInfo[i].st, listInfo[i].scoreList);

		cout << "|" << setfill('-') << setw(112) << "-" << "|" << endl;
		cout << setfill(' ');
	}
}