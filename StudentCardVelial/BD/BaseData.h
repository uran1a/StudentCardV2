﻿#pragma once

#include "../Classes/Faculty/Faculty.h"
#include "../Classes/Group/Group.h"
#include "../Classes/Student/Student.h"
#include "../Classes/Admin/Admin.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Data::SqlClient;

ref class BaseData
{
private:
	SqlConnection^ conn;
	SqlConnectionStringBuilder^ connStringBuilder;

private:
	//Подключение в базе данных MC Sql Server
	void ConnectToBD() {
		//Data Source=LAPTOP-N0SDJAIT\SQLEXPRESS;Initial Catalog=StudentCard;Integrated Security=True
		
		connStringBuilder = gcnew SqlConnectionStringBuilder();
		connStringBuilder->DataSource = "LAPTOP-N0SDJAIT\\SQLEXPRESS";
		connStringBuilder->InitialCatalog = "StudentCard";
		connStringBuilder->IntegratedSecurity = true;

		conn = gcnew SqlConnection(Convert::ToString(connStringBuilder));
	}
public:
	BaseData()
	{}
	//FACULTY
	//----------------------------------------
	void Insert(Faculty^ f) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "INSERT INTO dbo.TABLE_FACULTIES(Title_Faculty, Name_Dekan) VALUES(@TitleFaculty, @NameDekan)";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@TitleFaculty", f->TitleFaculty);
			cmd->Parameters->AddWithValue("@NameDekan", f->NameDekan);
			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При добавлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}

	}
	List<Faculty^>^ FillBaseData() {
		try {
			//Подключение в БД
			ConnectToBD();

			List<Faculty^>^ list = gcnew List<Faculty^>();

			String^ cmdText = "SELECT * FROM dbo.TABLE_FACULTIES";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				Faculty^ facl = gcnew Faculty();
				facl->ID = Convert::ToInt32(reader["ID"]->ToString());
				facl->TitleFaculty = (reader["Title_Faculty"]->ToString());
				facl->NameDekan = (reader["Name_Dekan"]->ToString());
				list->Add(facl);
			}
			return list;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	ListBox::ObjectCollection^ FillComboBox() {
		try {
			//Подключение в БД
			ConnectToBD();

			//List<Faculty^>^ list = gcnew List<Faculty^>();
			ListBox^ list = gcnew ListBox();


			String^ cmdText = "SELECT * FROM dbo.TABLE_FACULTIES";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				Faculty^ facl = gcnew Faculty();
				facl->ID = Convert::ToInt32(reader["ID"]->ToString());
				facl->TitleFaculty = (reader["Title_Faculty"]->ToString());
				//list->Add(facl);
				list->Items->Add(String::Format("{0}", facl->TitleFaculty));
			}
			return list->Items;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	void Reload(List<Faculty^>^% list, TreeView^ TreeViewFaculty) {
		//bd->Reload(list, bd, treeViewFaculty);
		//BaseData^ bd = gcnew BaseData();
		list = FillBaseData();
		//list_groups = FillListView(String ^ NameFaculty);

		TreeViewFaculty->Nodes->Clear();
		for (int i = 0; i < list->Count; i++) {
			List<Group^>^ list_groups = FillListView(list[i]->TitleFaculty);
			//Console::WriteLine("{0} {1}", list[i]->ID, list[i]->TitleFaculty);
			System::Windows::Forms::TreeNode^ newTreeNode = (gcnew System::Windows::Forms::TreeNode(list[i]->TitleFaculty));
			newTreeNode->Name = list[i]->TitleFaculty;
			newTreeNode->Text = list[i]->TitleFaculty;
			for (int j = 0; j < list_groups->Count; j++)
			{
				System::Windows::Forms::TreeNode^ newTreeSubNode = (gcnew System::Windows::Forms::TreeNode(Convert::ToString(list_groups[j]->TitleGroup)));
				//newTreeSubNode->Name = L"Узел5";
				//newTreeSubNode->Text = L"бИСТ-214";
				newTreeNode->Nodes->Add(newTreeSubNode);
			}
			TreeViewFaculty->Nodes->AddRange(gcnew cli::array< System::Windows::Forms::TreeNode^  >(1) { newTreeNode });
		}

	}
	void Delete(Faculty^ f) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "DELETE FROM dbo.TABLE_FACULTIES WHERE ID = @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@ID", f->ID);
			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При удалении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	void Update(Faculty^ f, int ID) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "UPDATE dbo.TABLE_FACULTIES SET Title_Faculty = @TitleFaculty, Name_Dekan = @NameDekan WHERE ID = @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@TitleFaculty", f->TitleFaculty);
			cmd->Parameters->AddWithValue("@NameDekan", f->NameDekan);
			cmd->Parameters->AddWithValue("@ID", ID);

			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При обновлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	bool Checking(Faculty^ f) {
		try {
			//Подключение в БД
			ConnectToBD();

			List<Faculty^>^ list = gcnew List<Faculty^>();
			bool checkingForMatches = false;
			String^ cmdText = "SELECT * FROM dbo.TABLE_FACULTIES";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				if (f->TitleFaculty->ToUpper()->Replace(" ", "") == (reader["Title_Faculty"]->ToString()->Replace(" ", "")->ToUpper()))
					return checkingForMatches = true;
			}
			return checkingForMatches;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	//----------------------------------------
	//GROUP
	//----------------------------------------
	void Insert(Group^ g) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "INSERT INTO dbo.TABLE_GROUPS(Title_Group, Title_Faculty, Name_Kurator, Name_Monitor, Number_Kurc, Specialization) VALUES(@Title_Group, @Title_Faculty, @Name_Kurator, @Name_Monitor, @Number_Kurc, @Specialization)";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@Title_Group", g->TitleGroup);
			cmd->Parameters->AddWithValue("@Title_Faculty", g->TitleFaculty);
			cmd->Parameters->AddWithValue("@Name_Kurator", g->NameKurator);
			cmd->Parameters->AddWithValue("@Name_Monitor", g->NameMonitor);
			cmd->Parameters->AddWithValue("@Number_Kurc", g->NumberKurc);
			cmd->Parameters->AddWithValue("@Specialization", g->Specialization);

			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При добавлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	List<Group^>^ FillListView(String^ NameFaculty) {
		try {
			//Подключение в БД
			ConnectToBD();

			List<Group^>^ list = gcnew List<Group^>();

			String^ cmdText = "SELECT * FROM dbo.TABLE_GROUPS WHERE Title_Faculty = @Title";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			cmd->Parameters->AddWithValue("@Title", NameFaculty);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				Group^ group = gcnew Group();
				group->ID = Convert::ToInt32(reader["ID"]->ToString());
				group->TitleGroup = (reader["Title_Group"]->ToString());
				group->TitleFaculty = (reader["Title_Faculty"]->ToString());
				group->NameKurator = (reader["Name_Kurator"]->ToString());
				group->NameMonitor = (reader["Name_Monitor"]->ToString());
				group->Specialization = (reader["Specialization"]->ToString());
				//group->NumberKurc = Convert::ToInt32(reader["ID"]->ToString());
				group->NumberKurc = Convert::ToInt32(reader["Number_Kurc"]->ToString());

				list->Add(group);
			}
			return list;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	void Reload(List<Group^>^% list_groups, ListView^ ListViewPanel, String^ NameFaculty) {
		list_groups = FillListView(NameFaculty);
		ListViewPanel->Items->Clear();
		for (int i = 0; i < list_groups->Count; i++) {
			//Console::WriteLine("{0} {1} {2}", list_groups[i]->ID, list_groups[i]->TitleGroup, list_groups[i]->TitleFaculty);
			ListViewItem^ newItem = gcnew ListViewItem(Convert::ToString(i + 1));
			ListViewItem::ListViewSubItem^ TitleGroup = gcnew ListViewItem::ListViewSubItem(newItem, list_groups[i]->TitleGroup);
			ListViewItem::ListViewSubItem^ NameKurator = gcnew ListViewItem::ListViewSubItem(newItem, list_groups[i]->NameKurator);
			newItem->SubItems->Add(TitleGroup);
			newItem->SubItems->Add(NameKurator);
			ListViewPanel->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(1) { newItem });
		}
	}
	void Delete(String^ TitleGroup) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "DELETE FROM dbo.TABLE_GROUPS WHERE Title_Group = @TitleGroup";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@TitleGroup", TitleGroup);
			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При удалении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	void Update(Group^ g, int ID) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "UPDATE dbo.TABLE_GROUPS SET Title_Group = @TitleGroup, Title_Faculty = @TitleFaculty, Name_Kurator = @NameKurator, Name_Monitor = @NameMonitor, Number_Kurc = @NumberKurc, Specialization = @Specialization WHERE ID = @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@TitleGroup", g->TitleGroup);
			cmd->Parameters->AddWithValue("@TitleFaculty", g->TitleFaculty);
			cmd->Parameters->AddWithValue("@NameKurator", g->NameKurator);
			cmd->Parameters->AddWithValue("@NameMonitor", g->NameMonitor);
			cmd->Parameters->AddWithValue("@NumberKurc", g->NumberKurc);
			cmd->Parameters->AddWithValue("@Specialization", g->Specialization);
			cmd->Parameters->AddWithValue("@ID", ID);

			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При обновлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	bool Checking(Group^ g) {
		try {
			//Подключение в БД
			ConnectToBD();

			List<Faculty^>^ list = gcnew List<Faculty^>();
			bool checkingForMatches = false;
			String^ cmdText = "SELECT * FROM dbo.TABLE_GROUPS";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				if (g->TitleGroup->ToUpper()->Replace(" ", "") == (reader["Title_Group"]->ToString()->Replace(" ", "")->ToUpper()))
					return checkingForMatches = true;
			}
			return checkingForMatches;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	//----------------------------------------
	//STUDENT
	//----------------------------------------
	void Insert(Student^ s) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "INSERT INTO dbo.TABLE_STUDENTS(Name, Surname, Middlename, Entrant, Title_Faculty, Title_Group, Birthday, Point_EGE, Stipendiya, Year_Enrollment, Photo_Student, Specialization, Educational_Form, Number_Kurc, Phone_Number, Mail, Otcenka, Login, Password) VALUES(@Name, @Surname, @Middlename, @Entrant, @Title_Faculty, @Title_Group, @Birthday, @Point_EGE, @Stipendiya, @Year_Enrollment, @Photo_Student, @Specialization, @Educational_Form, @Number_Kurc, @Phone_Number, @Mail, @Otcenka, @Login, @Password)";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@Name", s->Name);
			cmd->Parameters->AddWithValue("@Surname", s->Surname);
			cmd->Parameters->AddWithValue("@Middlename", s->Middlename);
			cmd->Parameters->AddWithValue("@Entrant", 1);
			cmd->Parameters->AddWithValue("@Title_Faculty", "");
			cmd->Parameters->AddWithValue("@Title_Group", "");
			cmd->Parameters->AddWithValue("@Stipendiya", 0);
			cmd->Parameters->AddWithValue("@Birthday", s->Birthday);
			cmd->Parameters->AddWithValue("@Point_EGE", s->Point_EGE);
			cmd->Parameters->AddWithValue("@Year_Enrollment", "");
			cmd->Parameters->AddWithValue("@Photo_Student", s->Photo_Student);
			cmd->Parameters->AddWithValue("@Specialization", "");
			cmd->Parameters->AddWithValue("@Educational_Form", s->Educational_Form);
			cmd->Parameters->AddWithValue("@Number_Kurc", "");
			cmd->Parameters->AddWithValue("@Phone_Number", s->Phone_Number);
			cmd->Parameters->AddWithValue("@Mail", s->Mail);
			cmd->Parameters->AddWithValue("@Otcenka", "0");
			cmd->Parameters->AddWithValue("@Login", s->Login);
			cmd->Parameters->AddWithValue("@Password", s->Password);

			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При добавлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	List<Student^>^ FillListViewStudent(String^ NameGroup) {
		try {
			//Подключение в БД
			ConnectToBD();

			List<Student^>^ list = gcnew List<Student^>();

			String^ cmdText = "SELECT * FROM dbo.TABLE_STUDENTS WHERE Title_Group = @Title";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			cmd->Parameters->AddWithValue("@Title", NameGroup);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				if (Convert::ToInt32(reader["Entrant"]) == 0) {
					Student^ student = gcnew Student();
					student->ID = Convert::ToInt32(reader["ID"]->ToString());
					student->Name = (reader["Name"]->ToString());
					student->Surname = (reader["Surname"]->ToString());
					student->Middlename = (reader["Middlename"]->ToString());
					student->Entrant = Convert::ToInt32(reader["Entrant"]);
					student->Title_Faculty = (reader["Title_Faculty"]->ToString());
					student->Title_Group = (reader["Title_Group"]->ToString());
					student->Birthday = (reader["Birthday"]->ToString());
					student->Point_EGE = Convert::ToInt32(reader["Point_EGE"]->ToString());
					student->Stipendiya = Convert::ToInt32(reader["Stipendiya"]->ToString());
					student->Year_Enrollment = (reader["Year_Enrollment"]->ToString());
					student->Photo_Student = (reader["Photo_Student"]->ToString());
					student->Specialization = (reader["Specialization"]->ToString());
					student->Educational_Form = (reader["Educational_Form"]->ToString());
					student->Number_Kurc = Convert::ToInt32(reader["Number_Kurc"]->ToString());
					student->Phone_Number = (reader["Phone_Number"]->ToString());
					student->Mail = (reader["Mail"]->ToString());
					student->Otcenka = Convert::ToDouble(reader["Otcenka"]->ToString());
					student->Login = (reader["Login"]->ToString());
					student->Password = (reader["Password"]->ToString());

					list->Add(student);
				}
			}
			return list;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	List<Student^>^ FillCheckedListBoxStudent(int StatusEntrant) {
		try {
			//Подключение в БД
			ConnectToBD();

			List<Student^>^ list = gcnew List<Student^>();

			String^ cmdText = "SELECT * FROM dbo.TABLE_STUDENTS WHERE Entrant = @Yes";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			cmd->Parameters->AddWithValue("@Yes", StatusEntrant);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				Student^ student = gcnew Student();
				student->ID = Convert::ToInt32(reader["ID"]->ToString());
				student->Name = (reader["Name"]->ToString());
				student->Surname = (reader["Surname"]->ToString());
				student->Middlename = (reader["Middlename"]->ToString());
				student->Entrant = Convert::ToInt32(reader["Entrant"]);
				student->Title_Faculty = (reader["Title_Faculty"]->ToString());
				student->Title_Group = (reader["Title_Group"]->ToString());
				student->Birthday = (reader["Birthday"]->ToString());
				student->Point_EGE = Convert::ToInt32(reader["Point_EGE"]->ToString());
				student->Stipendiya = Convert::ToInt32(reader["Stipendiya"]->ToString());
				student->Year_Enrollment = (reader["Year_Enrollment"]->ToString());
				student->Photo_Student = (reader["Photo_Student"]->ToString());
				student->Specialization = (reader["Specialization"]->ToString());
				student->Educational_Form = (reader["Educational_Form"]->ToString());
				student->Number_Kurc = Convert::ToInt32(reader["Number_Kurc"]->ToString());
				student->Phone_Number = (reader["Phone_Number"]->ToString());
				student->Mail = (reader["Mail"]->ToString());
				student->Otcenka = Convert::ToDouble(reader["Otcenka"]->ToString());
				student->Login = (reader["Login"]->ToString());
				student->Password = (reader["Password"]->ToString());

				list->Add(student);
			}
			return list;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	Student^ FillStudent(int ID) {
		try {
			//Подключение в БД
			ConnectToBD();;

			Student^ student = gcnew Student();

			String^ cmdText = "SELECT * FROM dbo.TABLE_STUDENTS WHERE ID = @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			cmd->Parameters->AddWithValue("@ID", ID);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			if (reader->Read()) {
				
				student->ID = Convert::ToInt32(reader["ID"]->ToString());
				student->Name = (reader["Name"]->ToString());
				student->Surname = (reader["Surname"]->ToString());
				student->Middlename = (reader["Middlename"]->ToString());
				student->Entrant = Convert::ToInt32(reader["Entrant"]);
				student->Title_Faculty = (reader["Title_Faculty"]->ToString());
				student->Title_Group = (reader["Title_Group"]->ToString());
				student->Birthday = (reader["Birthday"]->ToString());
				student->Point_EGE = Convert::ToInt32(reader["Point_EGE"]->ToString());
				student->Stipendiya = Convert::ToInt32(reader["Stipendiya"]->ToString());
				student->Year_Enrollment = (reader["Year_Enrollment"]->ToString());
				student->Photo_Student = (reader["Photo_Student"]->ToString());
				student->Specialization = (reader["Specialization"]->ToString());
				student->Educational_Form = (reader["Educational_Form"]->ToString());
				student->Number_Kurc = Convert::ToInt32(reader["Number_Kurc"]->ToString());
				student->Phone_Number = (reader["Phone_Number"]->ToString());
				student->Mail = (reader["Mail"]->ToString());
				student->Otcenka = Convert::ToDouble(reader["Otcenka"]->ToString());
				student->Login = (reader["Login"]->ToString());
				student->Password = (reader["Password"]->ToString());
			}
			return student;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	void Reload(List<Student^>^% list_student, ListView^ ListViewPanel, String^ NameGroup) {
		list_student = FillListViewStudent(NameGroup);
		ListViewPanel->Items->Clear();
		for (int i = 0; i < list_student->Count; i++) {
			ListViewItem^ newItem = gcnew ListViewItem(Convert::ToString(i + 1));
			ListViewItem::ListViewSubItem^ Name = gcnew ListViewItem::ListViewSubItem(newItem, list_student[i]->Name);
			ListViewItem::ListViewSubItem^ Surname = gcnew ListViewItem::ListViewSubItem(newItem, list_student[i]->Surname);
			ListViewItem::ListViewSubItem^ Middlename = gcnew ListViewItem::ListViewSubItem(newItem, list_student[i]->Middlename);
			newItem->SubItems->Add(Surname);
			newItem->SubItems->Add(Name);
			newItem->SubItems->Add(Middlename);
			ListViewPanel->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(1) { newItem });
		}
	}
	void Reload(List<Student^>^% list_student, CheckedListBox^ CheckedListBoxStudent) {
		list_student = FillCheckedListBoxStudent(1);
		//Добавить столбец балл ЕГЭ с сортировкой
		CheckedListBoxStudent->Items->Clear();
		for (size_t i = 0; i < list_student->Count; i++)
		{
			CheckedListBoxStudent->Items->AddRange(gcnew cli::array< System::Object^  >(1) { Convert::ToString(list_student[i]->Surname) });
		}
	}
	void Reload(List<Student^>^% list_student, ListView^ ListViewPanel) {
		list_student = FillCheckedListBoxStudent(0);
		ListViewPanel->FullRowSelect = true;
		ListViewPanel->Items->Clear();
		for (int i = 0; i < list_student->Count; i++) {
			ListViewItem^ newItem = gcnew ListViewItem(Convert::ToString(i + 1));
			ListViewItem::ListViewSubItem^ Login = gcnew ListViewItem::ListViewSubItem(newItem, list_student[i]->Login);
			ListViewItem::ListViewSubItem^ Password = gcnew ListViewItem::ListViewSubItem(newItem, list_student[i]->Password);
			newItem->SubItems->Add(Login);
			newItem->SubItems->Add(Password);
			ListViewPanel->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(1) { newItem });
		}
	}
	void Delete(Student^ student) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "DELETE FROM dbo.TABLE_STUDENTS WHERE ID = @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@ID", student->ID);
			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При удалении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	void Update(Student^ s) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "UPDATE dbo.TABLE_STUDENTS SET Entrant = @Entrant, Title_Faculty = @Title_Faculty, Title_Group = @Title_Group, Specialization = @Specialization, Number_Kurc = @Number_Kurc WHERE ID = @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@ID", s->ID);
			cmd->Parameters->AddWithValue("@Entrant", s->Entrant);
			cmd->Parameters->AddWithValue("@Title_Group", s->Title_Group);
			cmd->Parameters->AddWithValue("@Title_Faculty", s->Title_Faculty);
			cmd->Parameters->AddWithValue("@Specialization", s->Specialization);
			cmd->Parameters->AddWithValue("@Number_Kurc", s->Number_Kurc);

			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При обновлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	void Update(Student^ s, int ID) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "UPDATE dbo.TABLE_STUDENTS SET Name = @Name, Surname = @Surname, Middlename = @Middlename, Title_Group = @Title_Group, Birthday = @Birthday, Point_EGE = @Point_EGE, Stipendiya = @Stipendiya, Year_Enrollment = @Year_Enrollment, Otcenka = @Otcenka, Photo_Student = @Photo_Student,  Educational_Form = @Educational_Form, Phone_Number = @Phone_Number, Mail = @Mail WHERE ID = @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@ID", ID);
			//cmd->Parameters->AddWithValue("@Entrant", s->Entrant);
			cmd->Parameters->AddWithValue("@Name", s->Name);
			cmd->Parameters->AddWithValue("@Surname", s->Surname);
			cmd->Parameters->AddWithValue("@Middlename", s->Middlename);
			cmd->Parameters->AddWithValue("@Title_Group", s->Title_Group);
			cmd->Parameters->AddWithValue("@Birthday", s->Birthday);
			cmd->Parameters->AddWithValue("@Point_EGE", s->Point_EGE);
			cmd->Parameters->AddWithValue("@Stipendiya", s->Stipendiya);
			cmd->Parameters->AddWithValue("@Year_Enrollment", s->Year_Enrollment);
			cmd->Parameters->AddWithValue("@Otcenka", Convert::ToString(s->Otcenka));
			cmd->Parameters->AddWithValue("@Photo_Student", s->Photo_Student);
			//cmd->Parameters->AddWithValue("@Specialization", s->Specialization);
			cmd->Parameters->AddWithValue("@Educational_Form", s->Educational_Form);
			//cmd->Parameters->AddWithValue("@Number_Kurc", s->Number_Kurc);
			cmd->Parameters->AddWithValue("@Phone_Number", s->Phone_Number);
			cmd->Parameters->AddWithValue("@Mail", s->Mail);

			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При обновлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	void Update(int Entrant, String^ PathGroup) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "UPDATE dbo.TABLE_STUDENTS SET Entrant = @Entrant WHERE Title_Group = @Title_Group";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			//cmd->Parameters->AddWithValue("@ID", ID);
			cmd->Parameters->AddWithValue("@Entrant", Entrant);
			cmd->Parameters->AddWithValue("@Title_Group", PathGroup);

			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При обновлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	bool Checking(Student^ s) {
		try {
			//Подключение в БД
			ConnectToBD();

			bool checkingForMatches = false;
			String^ cmdText = "SELECT * FROM dbo.TABLE_STUDENTS WHERE ID != @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			cmd->Parameters->AddWithValue("@ID", s->ID);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				if (s->Name->ToUpper()->Replace(" ", "") == (reader["Name"]->ToString()->Replace(" ", "")->ToUpper()) && s->Surname->ToUpper()->Replace(" ", "") == (reader["Surname"]->ToString()->Replace(" ", "")->ToUpper()) && s->Middlename->ToUpper()->Replace(" ", "") == (reader["Middlename"]->ToString()->Replace(" ", "")->ToUpper()))
					return checkingForMatches = true;
			}
			return checkingForMatches;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	bool CheckingPassword(Student^ s) {
		try {
			//Подключение в БД
			ConnectToBD();

			bool checkingForMatches = false;
			String^ cmdText = "SELECT * FROM dbo.TABLE_STUDENTS WHERE ID != @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			cmd->Parameters->AddWithValue("@ID", s->ID);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				if (s->Login->ToUpper()->Replace(" ", "") == (reader["Login"]->ToString()->Replace(" ", "")->ToUpper()) && s->Password->ToUpper()->Replace(" ", "") == (reader["Password"]->ToString()->Replace(" ", "")->ToUpper()))
					return checkingForMatches = true;
			}
			return checkingForMatches;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	//----------------------------------------
	//AUTORIZATION
	//----------------------------------------
	int SignOn(int% ID, String^ Login, String^ Password) {
	
			//Подключение в БД
			ConnectToBD();

			int isAdmin = 0;

			String^ cmdText = "SELECT * FROM dbo.TABLE_STUDENTS";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				if (Login == reader["Login"]->ToString() && Password == reader["Password"]->ToString()) {
					ID = Convert::ToInt32(reader["ID"]);
					return isAdmin = 1;
				}
			}
			if (conn != nullptr) conn->Close();

			cmdText = "SELECT * FROM dbo.TABLE_ADMINS";
			cmd = gcnew SqlCommand(cmdText, conn);
			conn->Open();

			reader = cmd->ExecuteReader();
			while (reader->Read()) {
				if (Login == reader["Login"]->ToString() && Password == reader["Password"]->ToString()) {
					ID = Convert::ToInt32(reader["ID"]);
					return isAdmin = 2;
				}
			}
			if (conn != nullptr)
				conn->Close();

			if (!isAdmin) return isAdmin;
		
	}
	void SignIn(String^ Login, String^ Password) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "INSERT INTO dbo.TABLE_USERS(Login, Password, Type) VALUES(@Login, @Password, @Type)";
			
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@Login", Login);
			cmd->Parameters->AddWithValue("@Password", Password);
			cmd->Parameters->AddWithValue("@Type", "User");
			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При добавлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
			MessageBox::Show("Вы успешно зарегистрировались!");
		}
		//try
	}
	//----------------------------------------
	//ADMIN
	//----------------------------------------
	void Insert(Admin^ a) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "INSERT INTO dbo.TABLE_ADMINS(Name, Surname, Patronymic, Birthday, Dolzhnost, Photo, Stazh, Zarplata, Mobile_Phone, Mail, Login, Password) VALUES(@Name, @Surname, @Patronymic, @Birthday, @Dolzhnost, @Photo, @Stazh, @Zarplata, @Mobile_Phone, @Mail, @Login, @Password)";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@Name", a->Name);
			cmd->Parameters->AddWithValue("@Surname", a->Surname);
			cmd->Parameters->AddWithValue("@Patronymic", a->Patronymic);
			cmd->Parameters->AddWithValue("@Birthday", a->Birthday);
			cmd->Parameters->AddWithValue("@Dolzhnost", a->Dolzhnost);
			cmd->Parameters->AddWithValue("@Photo", a->Photo);
			cmd->Parameters->AddWithValue("@Stazh", a->Stazh);
			cmd->Parameters->AddWithValue("@Zarplata", a->Zarplata);
			cmd->Parameters->AddWithValue("@Mobile_Phone", a->Mobile_Phone);
			cmd->Parameters->AddWithValue("@Mail", a->Mail);
			cmd->Parameters->AddWithValue("@Login", a->Login);
			cmd->Parameters->AddWithValue("@Password", a->Password);

			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При добавлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	Admin^ FillAdmin(int ID) {
		try {
			//Подключение в БД
			ConnectToBD();

			Admin^ admin = gcnew Admin();

			String^ cmdText = "SELECT * FROM dbo.TABLE_ADMINS WHERE ID = @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			cmd->Parameters->AddWithValue("@ID", ID);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			if (reader->Read()) {
				admin->ID = Convert::ToInt32(reader["ID"]->ToString());
				admin->Name = (reader["Name"]->ToString());
				admin->Surname = (reader["Surname"]->ToString());
				admin->Patronymic = (reader["Patronymic"]->ToString());
				admin->Birthday = (reader["Birthday"]->ToString());
				admin->Dolzhnost = (reader["Dolzhnost"]->ToString());
				admin->Stazh = Convert::ToInt32(reader["Stazh"]->ToString());
				admin->Zarplata = Convert::ToInt32(reader["Zarplata"]->ToString());
				admin->Photo = (reader["Photo"]->ToString());
				admin->Mobile_Phone = (reader["Mobile_Phone"]->ToString());
				admin->Mail = (reader["Mail"]->ToString());
				admin->Login = (reader["Login"]->ToString());
				admin->Password = (reader["Password"]->ToString());
			}
			return admin;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	List<Admin^>^ FillListViewAdmins() {
		try {
			//Ïîäêëþ÷åíèå â ÁÄ
			ConnectToBD();

			List<Admin^>^ list = gcnew List<Admin^>();

			String^ cmdText = "SELECT * FROM dbo.TABLE_ADMINS";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			cmd->Parameters->AddWithValue("@Type", "User      ");
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
			
				Admin^ admin = gcnew Admin();
				admin->ID = Convert::ToInt32(reader["ID"]->ToString());
				admin->Login = (reader["Login"]->ToString());
				admin->Password = (reader["Password"]->ToString());

				list->Add(admin);
			}
			return list;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Îøèáêà: Ïðè ÷òåíèè ýëåìåíòîâ èç ÁÄ!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	void Reload(List<Admin^>^% list_admin, ListView^ ListViewPanel) {
		list_admin = FillListViewAdmins();
		ListViewPanel->FullRowSelect = true;
		ListViewPanel->Items->Clear();
		for (int i = 0; i < list_admin->Count; i++) {
			ListViewItem^ newItem = gcnew ListViewItem(Convert::ToString(i + 1));
			ListViewItem::ListViewSubItem^ Login = gcnew ListViewItem::ListViewSubItem(newItem, list_admin[i]->Login);
			ListViewItem::ListViewSubItem^ Password = gcnew ListViewItem::ListViewSubItem(newItem, list_admin[i]->Password);
			newItem->SubItems->Add(Login);
			newItem->SubItems->Add(Password);
			ListViewPanel->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(1) { newItem });
		}
	}
	void Update(Admin^ a, int ID) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "UPDATE dbo.TABLE_ADMINS SET Name = @Name, Surname = @Surname, Patronymic = @Patronymic, Birthday = @Birthday, Dolzhnost = @Dolzhnost, Photo = @Photo, Stazh = @Stazh, Zarplata = @Zarplata, Mobile_Phone = @Mobile_Phone,  Mail = @Mail, Login = @Login, Password = @Password WHERE ID = @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@ID", ID);
			cmd->Parameters->AddWithValue("@Name", a->Name);
			cmd->Parameters->AddWithValue("@Surname", a->Surname);
			cmd->Parameters->AddWithValue("@Patronymic", a->Patronymic);
			cmd->Parameters->AddWithValue("@Birthday", a->Birthday);
			cmd->Parameters->AddWithValue("@Dolzhnost", a->Dolzhnost);
			cmd->Parameters->AddWithValue("@Photo", a->Photo);
			cmd->Parameters->AddWithValue("@Stazh", a->Stazh);
			cmd->Parameters->AddWithValue("@Zarplata", a->Zarplata);
			cmd->Parameters->AddWithValue("@Mobile_Phone", a->Mobile_Phone);
			cmd->Parameters->AddWithValue("@Mail", a->Mail);
			cmd->Parameters->AddWithValue("@Login", a->Login);
			cmd->Parameters->AddWithValue("@Password", a->Password);

			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При обновлении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	void Delete(Admin^ admin) {
		try {
			//Подключение в БД
			ConnectToBD();

			String^ cmdText = "DELETE FROM dbo.TABLE_ADMINS WHERE ID = @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);

			cmd->Parameters->AddWithValue("@ID", admin->ID);
			conn->Open();
			cmd->ExecuteNonQuery();
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При удалении элемента в БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	bool Checking(Admin^ a) {
		try {
			//Подключение в БД
			ConnectToBD();

			bool checkingForMatches = false;
			String^ cmdText = "SELECT * FROM dbo.TABLE_ADMINS WHERE ID != @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			cmd->Parameters->AddWithValue("@ID", a->ID);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				if (a->Name->ToUpper()->Replace(" ", "") == (reader["Name"]->ToString()->Replace(" ", "")->ToUpper()) && a->Surname->ToUpper()->Replace(" ", "") == (reader["Surname"]->ToString()->Replace(" ", "")->ToUpper()) && a->Patronymic->ToUpper()->Replace(" ", "") == (reader["Patronymic"]->ToString()->Replace(" ", "")->ToUpper()))
					return checkingForMatches = true;
			}
			return checkingForMatches;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	bool CheckingPassword(Admin^ a) {
		try {
			//Подключение в БД
			ConnectToBD();

			bool checkingForMatches = false;
			String^ cmdText = "SELECT * FROM dbo.TABLE_ADMINS WHERE ID != @ID";
			SqlCommand^ cmd = gcnew SqlCommand(cmdText, conn);
			cmd->Parameters->AddWithValue("@ID", a->ID);
			conn->Open();

			SqlDataReader^ reader = cmd->ExecuteReader();
			while (reader->Read()) {
				if (a->Login->ToUpper()->Replace(" ", "") == (reader["Login"]->ToString()->Replace(" ", "")->ToUpper()) || a->Password->ToUpper()->Replace(" ", "") == (reader["Password"]->ToString()->Replace(" ", "")->ToUpper()))
					return checkingForMatches = true;
			}
			return checkingForMatches;
		}
		finally {
			if (conn != nullptr)
				conn->Close();
			else MessageBox::Show("Ошибка: При чтении элементов из БД!", "Help", MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
		}
	}
	//----------------------------------------


};

