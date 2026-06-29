#pragma once
#include <msclr/marshal.h>

namespace SUBD {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Collections::Generic;
    using namespace System::Data::Odbc;

    public ref class TrainingParticipantsForm : public System::Windows::Forms::Form
    {
    public:
        TrainingParticipantsForm(int trainingId, String^ trainingInfo, String^ connStr)
        {
            this->trainingId = trainingId;
            this->trainingInfo = trainingInfo;
            this->connStr = connStr;
            InitializeComponent();
            LoadParticipants();
            ApplyModernDesign();
        }

    private:
        int trainingId;
        String^ trainingInfo;
        String^ connStr;
        DataGridView^ dataGridParticipants;
        Button^ btnClose;
        Label^ lblTitle;

        void InitializeComponent()
        {
            this->Text = "Участники тренировки";
            this->Size = Drawing::Size(600, 500);
            this->StartPosition = FormStartPosition::CenterParent;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;

            // Заголовок
            lblTitle = gcnew Label();
            lblTitle->Text = "Участники тренировки: " + trainingInfo;
            lblTitle->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
            lblTitle->Location = Point(20, 20);
            lblTitle->Size = Drawing::Size(550, 25);
            this->Controls->Add(lblTitle);

            // Таблица участников
            dataGridParticipants = gcnew DataGridView();
            dataGridParticipants->Location = Point(20, 60);
            dataGridParticipants->Size = Drawing::Size(550, 350);
            dataGridParticipants->ReadOnly = true;
            dataGridParticipants->AllowUserToAddRows = false;
            dataGridParticipants->AllowUserToDeleteRows = false;
            dataGridParticipants->AllowUserToOrderColumns = false;
            this->Controls->Add(dataGridParticipants);

            // Кнопка закрытия
            btnClose = gcnew Button();
            btnClose->Text = "Закрыть";
            btnClose->Location = Point(470, 420);
            btnClose->Size = Drawing::Size(100, 35);
            btnClose->Click += gcnew EventHandler(this, &TrainingParticipantsForm::btnClose_Click);
            this->Controls->Add(btnClose);
        }

        void LoadParticipants()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                OdbcCommand^ cmd = gcnew OdbcCommand(
                    "SELECT participants FROM trainings WHERE training_id = ?;", conn);
                cmd->Parameters->AddWithValue("training_id", trainingId);
                conn->Open();

                Object^ result = cmd->ExecuteScalar();
                conn->Close();

                if (result != nullptr && result != DBNull::Value) {
                    String^ participantsString = result->ToString();

                    // Создаем DataTable для отображения
                    DataTable^ dt = gcnew DataTable();
                    dt->Columns->Add("№", int::typeid);
                    dt->Columns->Add("Имя игрока", String::typeid);

                    // Разделяем строку участников
                    array<String^>^ participants = participantsString->Split(',');

                    int counter = 1;
                    for each (String ^ participant in participants) {
                        String^ trimmedName = participant->Trim();
                        if (!String::IsNullOrEmpty(trimmedName)) {
                            dt->Rows->Add(counter++, trimmedName);
                        }
                    }

                    dataGridParticipants->DataSource = dt;

                    // Настраиваем внешний вид таблицы
                    dataGridParticipants->Columns["№"]->Width = 50;
                    dataGridParticipants->Columns["Имя игрока"]->AutoSizeMode = DataGridViewAutoSizeColumnMode::Fill;
                }
                else {
                    DataTable^ dt = gcnew DataTable();
                    dt->Columns->Add("Сообщение", String::typeid);
                    dt->Rows->Add("Участники не указаны");
                    dataGridParticipants->DataSource = dt;
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки участников: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void ApplyModernDesign()
        {
            this->BackColor = Color::FromArgb(245, 247, 250);
            this->Font = gcnew Drawing::Font("Segoe UI", 9);

            // Стилизация кнопки
            btnClose->FlatStyle = FlatStyle::Flat;
            btnClose->FlatAppearance->BorderSize = 0;
            btnClose->BackColor = Color::FromArgb(108, 117, 125);
            btnClose->ForeColor = Color::White;
            btnClose->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            btnClose->Cursor = Cursors::Hand;

            // Стилизация таблицы
            dataGridParticipants->BackgroundColor = Color::White;
            dataGridParticipants->BorderStyle = BorderStyle::None;
            dataGridParticipants->EnableHeadersVisualStyles = false;

            // Заголовки
            dataGridParticipants->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(41, 128, 185);
            dataGridParticipants->ColumnHeadersDefaultCellStyle->ForeColor = Color::White;
            dataGridParticipants->ColumnHeadersDefaultCellStyle->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            dataGridParticipants->ColumnHeadersHeight = 40;
            dataGridParticipants->ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle::None;

            // Строки
            dataGridParticipants->DefaultCellStyle->Font = gcnew Drawing::Font("Segoe UI", 9);
            dataGridParticipants->DefaultCellStyle->BackColor = Color::White;
            dataGridParticipants->DefaultCellStyle->ForeColor = Color::FromArgb(60, 60, 60);
            dataGridParticipants->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(248, 250, 252);

            dataGridParticipants->RowHeadersVisible = false;
            dataGridParticipants->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::None;
            dataGridParticipants->RowTemplate->Height = 35;
        }

        void btnClose_Click(Object^ sender, EventArgs^ e)
        {
            this->Close();
        }
    };
}