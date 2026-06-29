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

    public ref class MatchLineupForm : public System::Windows::Forms::Form
    {
    public:
        MatchLineupForm(int matchId, String^ matchInfo, String^ connStr)
        {
            this->matchId = matchId;
            this->matchInfo = matchInfo;
            this->connStr = connStr;
            InitializeComponent();
            LoadLineup();
            ApplyModernDesign();
        }

    private:
        int matchId;
        String^ matchInfo;
        String^ connStr;
        DataGridView^ dataGridLineup;
        Button^ btnClose;
        Label^ lblTitle;

        void InitializeComponent()
        {
            this->Text = "Состав на матч";
            this->Size = Drawing::Size(700, 500);
            this->StartPosition = FormStartPosition::CenterParent;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;

            // Заголовок
            lblTitle = gcnew Label();
            lblTitle->Text = "Состав на матч: " + matchInfo;
            lblTitle->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
            lblTitle->Location = Point(20, 20);
            lblTitle->Size = Drawing::Size(650, 25);
            this->Controls->Add(lblTitle);

            // Таблица состава
            dataGridLineup = gcnew DataGridView();
            dataGridLineup->Location = Point(20, 60);
            dataGridLineup->Size = Drawing::Size(650, 350);
            dataGridLineup->ReadOnly = true;
            dataGridLineup->AllowUserToAddRows = false;
            dataGridLineup->AllowUserToDeleteRows = false;
            dataGridLineup->AllowUserToOrderColumns = false;
            this->Controls->Add(dataGridLineup);

            // Кнопка закрытия
            btnClose = gcnew Button();
            btnClose->Text = "Закрыть";
            btnClose->Location = Point(570, 420);
            btnClose->Size = Drawing::Size(100, 35);
            btnClose->Click += gcnew EventHandler(this, &MatchLineupForm::btnClose_Click);
            this->Controls->Add(btnClose);
        }

        void LoadLineup()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                OdbcCommand^ cmd = gcnew OdbcCommand(
                    "SELECT player_name, position, is_starter FROM matchlineups WHERE match_id = ? ORDER BY is_starter DESC, player_name;", conn);
                cmd->Parameters->AddWithValue("match_id", matchId);
                conn->Open();

                OdbcDataReader^ reader = cmd->ExecuteReader();

                // Создаем DataTable для отображения
                DataTable^ dt = gcnew DataTable();
                dt->Columns->Add("№", int::typeid);
                dt->Columns->Add("Игрок", String::typeid);
                dt->Columns->Add("Позиция", String::typeid);
                dt->Columns->Add("Статус", String::typeid);

                int counter = 1;
                while (reader->Read()) {
                    String^ playerName = reader->GetString(0);
                    String^ position = reader->GetString(1);
                    bool isStarter = reader->GetBoolean(2);
                    String^ status = isStarter ? "Основной" : "Запасной";

                    dt->Rows->Add(counter++, playerName, position, status);
                }

                reader->Close();
                conn->Close();

                dataGridLineup->DataSource = dt;

                // Настраиваем внешний вид таблицы
                dataGridLineup->Columns["№"]->Width = 50;
                dataGridLineup->Columns["Игрок"]->Width = 200;
                dataGridLineup->Columns["Позиция"]->Width = 200;
                dataGridLineup->Columns["Статус"]->Width = 100;

            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки состава: " + ex->Message, "Ошибка",
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
            dataGridLineup->BackgroundColor = Color::White;
            dataGridLineup->BorderStyle = BorderStyle::None;
            dataGridLineup->EnableHeadersVisualStyles = false;

            // Заголовки
            dataGridLineup->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(41, 128, 185);
            dataGridLineup->ColumnHeadersDefaultCellStyle->ForeColor = Color::White;
            dataGridLineup->ColumnHeadersDefaultCellStyle->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            dataGridLineup->ColumnHeadersHeight = 40;
            dataGridLineup->ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle::None;

            // Строки
            dataGridLineup->DefaultCellStyle->Font = gcnew Drawing::Font("Segoe UI", 9);
            dataGridLineup->DefaultCellStyle->BackColor = Color::White;
            dataGridLineup->DefaultCellStyle->ForeColor = Color::FromArgb(60, 60, 60);
            dataGridLineup->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(248, 250, 252);

            dataGridLineup->RowHeadersVisible = false;
            dataGridLineup->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::None;
            dataGridLineup->RowTemplate->Height = 35;
        }

        void btnClose_Click(Object^ sender, EventArgs^ e)
        {
            this->Close();
        }
    };
}