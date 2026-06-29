#pragma once
#include <msclr/marshal.h>

namespace SUBD {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Data::Odbc;
    using namespace System::Drawing;
    using namespace System::IO;
    using namespace System::Text;

    public ref class MatchEventsViewForm : public System::Windows::Forms::Form
    {
    public:
        MatchEventsViewForm(int matchId, String^ matchInfo, String^ connectionString)
        {
            this->matchId = matchId;
            this->matchInfo = matchInfo;
            this->connStr = connectionString;
            InitializeComponent();
            LoadMatchEvents();
        }

    protected:
        ~MatchEventsViewForm()
        {
            if (components) delete components;
        }

    private:
        System::ComponentModel::Container^ components;

        // Поля формы
        int matchId;
        String^ matchInfo;
        String^ connStr;

        // Элементы управления
        Label^ lblMatchInfo;
        DataGridView^ dataGridEvents;
        Button^ btnClose;
        Button^ btnExport;

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->Size = System::Drawing::Size(900, 600);
            this->StartPosition = FormStartPosition::CenterParent;
            this->Text = "События матча";
            this->Padding = System::Windows::Forms::Padding(10);
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;

            // Заголовок с информацией о матче
            lblMatchInfo = gcnew Label();
            lblMatchInfo->Text = "Матч: " + matchInfo;
            lblMatchInfo->Font = gcnew System::Drawing::Font("Segoe UI", 12, FontStyle::Bold);
            lblMatchInfo->ForeColor = Color::FromArgb(41, 128, 185);
            lblMatchInfo->AutoSize = true;
            lblMatchInfo->Location = Point(10, 10);

            // DataGridView для событий
            dataGridEvents = gcnew DataGridView();
            dataGridEvents->Location = Point(10, 50);
            dataGridEvents->Size = System::Drawing::Size(860, 450);
            dataGridEvents->AllowUserToAddRows = false;
            dataGridEvents->AllowUserToDeleteRows = false;
            dataGridEvents->AllowUserToOrderColumns = false;
            dataGridEvents->ReadOnly = true;
            dataGridEvents->RowHeadersVisible = false;
            dataGridEvents->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dataGridEvents->MultiSelect = false;
            dataGridEvents->BackgroundColor = Color::White;
            dataGridEvents->BorderStyle = BorderStyle::FixedSingle;
            dataGridEvents->AutoGenerateColumns = false; // Важное исправление

            // Настройка колонок
            InitializeDataGridColumns();

            // Кнопка экспорта
            btnExport = gcnew Button();
            btnExport->Text = "Экспорт";
            btnExport->Size = System::Drawing::Size(120, 35);
            btnExport->Location = Point(650, 510);
            btnExport->BackColor = Color::FromArgb(46, 204, 113);
            btnExport->ForeColor = Color::White;
            btnExport->FlatStyle = FlatStyle::Flat;
            btnExport->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Bold);

            // Кнопка закрытия
            btnClose = gcnew Button();
            btnClose->Text = "Закрыть";
            btnClose->Size = System::Drawing::Size(120, 35);
            btnClose->Location = Point(780, 510);
            btnClose->BackColor = Color::FromArgb(108, 117, 125);
            btnClose->ForeColor = Color::White;
            btnClose->FlatStyle = FlatStyle::Flat;
            btnClose->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Bold);

            // Добавление элементов на форму
            this->Controls->Add(lblMatchInfo);
            this->Controls->Add(dataGridEvents);
            this->Controls->Add(btnExport);
            this->Controls->Add(btnClose);

            // Обработчики событий
            btnExport->Click += gcnew EventHandler(this, &MatchEventsViewForm::btnExport_Click);
            btnClose->Click += gcnew EventHandler(this, &MatchEventsViewForm::btnClose_Click);

            // Стилизация DataGridView
            StyleDataGridView();
        }

        void InitializeDataGridColumns()
        {
            dataGridEvents->Columns->Clear();

            // Колонка для игрока
            DataGridViewTextBoxColumn^ playerColumn = gcnew DataGridViewTextBoxColumn();
            playerColumn->HeaderText = "Игрок";
            playerColumn->Name = "Player";
            playerColumn->DataPropertyName = "player_name"; // Привязка к полю DataTable
            playerColumn->Width = 150;
            playerColumn->ReadOnly = true;
            dataGridEvents->Columns->Add(playerColumn);

            // Колонка для типа события
            DataGridViewTextBoxColumn^ eventTypeColumn = gcnew DataGridViewTextBoxColumn();
            eventTypeColumn->HeaderText = "Тип события";
            eventTypeColumn->Name = "EventType";
            eventTypeColumn->DataPropertyName = "event_type"; // Привязка к полю DataTable
            eventTypeColumn->Width = 150;
            eventTypeColumn->ReadOnly = true;
            dataGridEvents->Columns->Add(eventTypeColumn);

            // Колонка для минуты
            DataGridViewTextBoxColumn^ minuteColumn = gcnew DataGridViewTextBoxColumn();
            minuteColumn->HeaderText = "Минута";
            minuteColumn->Name = "Minute";
            minuteColumn->DataPropertyName = "minute"; // Привязка к полю DataTable
            minuteColumn->Width = 80;
            minuteColumn->ReadOnly = true;
            dataGridEvents->Columns->Add(minuteColumn);

            // Колонка для деталей
            DataGridViewTextBoxColumn^ detailsColumn = gcnew DataGridViewTextBoxColumn();
            detailsColumn->HeaderText = "Детали";
            detailsColumn->Name = "Details";
            detailsColumn->DataPropertyName = "details"; // Привязка к полю DataTable
            detailsColumn->Width = 350;
            detailsColumn->ReadOnly = true;
            dataGridEvents->Columns->Add(detailsColumn);
        }

        void StyleDataGridView()
        {
            dataGridEvents->BackgroundColor = Color::White;
            dataGridEvents->BorderStyle = BorderStyle::FixedSingle;
            dataGridEvents->EnableHeadersVisualStyles = false;

            // Стиль заголовков
            dataGridEvents->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(41, 128, 185);
            dataGridEvents->ColumnHeadersDefaultCellStyle->ForeColor = Color::White;
            dataGridEvents->ColumnHeadersDefaultCellStyle->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            dataGridEvents->ColumnHeadersHeight = 35;

            // Стиль строк
            dataGridEvents->DefaultCellStyle->Font = gcnew System::Drawing::Font("Segoe UI", 9);
            dataGridEvents->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(248, 250, 252);
            dataGridEvents->RowTemplate->Height = 30;

            // Стиль выделения
            dataGridEvents->DefaultCellStyle->SelectionBackColor = Color::FromArgb(229, 243, 255);
            dataGridEvents->DefaultCellStyle->SelectionForeColor = Color::Black;
        }

        void LoadMatchEvents()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                String^ query =
                    "SELECT player_name, event_type, minute, details " +
                    "FROM matchevents " +
                    "WHERE match_id = ? " +
                    "ORDER BY minute, event_id;";

                OdbcCommand^ cmd = gcnew OdbcCommand(query, conn);
                cmd->Parameters->AddWithValue("match_id", matchId);

                OdbcDataAdapter^ da = gcnew OdbcDataAdapter(cmd);
                DataTable^ dt = gcnew DataTable();
                da->Fill(dt);

                dataGridEvents->DataSource = dt;

                // Подсветка важных событий
                for each (DataGridViewRow ^ row in dataGridEvents->Rows) {
                    if (row->IsNewRow) continue;

                    String^ eventType = safe_cast<String^>(row->Cells["EventType"]->Value);
                    if (eventType != nullptr) {
                        if (eventType == "Гол") {
                            row->DefaultCellStyle->BackColor = Color::FromArgb(204, 255, 204); // Зеленый для голов
                            row->DefaultCellStyle->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Bold);
                        }
                        else if (eventType == "Желтая карточка") {
                            row->DefaultCellStyle->BackColor = Color::FromArgb(255, 255, 153); // Желтый для желтых карточек
                        }
                        else if (eventType == "Красная карточка") {
                            row->DefaultCellStyle->BackColor = Color::FromArgb(255, 153, 153); // Красный для красных карточек
                            row->DefaultCellStyle->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Bold);
                        }
                    }
                }

                // Обновляем заголовок с количеством событий
                lblMatchInfo->Text = String::Format("Матч: {0} (Событий: {1})", matchInfo, dt->Rows->Count);

            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки событий матча: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        // Обработчики событий
        System::Void btnExport_Click(System::Object^ sender, System::EventArgs^ e)
        {
            try {
                SaveFileDialog^ saveDialog = gcnew SaveFileDialog();
                saveDialog->Filter = "CSV файлы (*.csv)|*.csv|Текстовые файлы (*.txt)|*.txt";
                saveDialog->FileName = String::Format("События_матча_{0}.csv", matchId);
                saveDialog->Title = "Экспорт событий матча";

                if (saveDialog->ShowDialog() == Windows::Forms::DialogResult::OK) {
                    ExportToCsv(saveDialog->FileName);
                    MessageBox::Show("Данные успешно экспортированы!", "Успех",
                        MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка экспорта: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnClose_Click(System::Object^ sender, System::EventArgs^ e)
        {
            this->Close();
        }

        void ExportToCsv(String^ filePath)
        {
            StreamWriter^ sw = gcnew StreamWriter(filePath, false, System::Text::Encoding::UTF8);

            // Заголовок
            sw->WriteLine("Матч: {0}", matchInfo);
            sw->WriteLine("Дата экспорта: {0:dd.MM.yyyy HH:mm}", DateTime::Now);
            sw->WriteLine();

            // Заголовки колонок
            sw->WriteLine("Игрок;Тип события;Минута;Детали");

            // Данные
            for each (DataGridViewRow ^ row in dataGridEvents->Rows) {
                if (row->IsNewRow) continue;

                String^ player = EscapeCsv(safe_cast<String^>(row->Cells["Player"]->Value));
                String^ eventType = EscapeCsv(safe_cast<String^>(row->Cells["EventType"]->Value));
                String^ minute = EscapeCsv(safe_cast<String^>(row->Cells["Minute"]->Value));
                String^ details = EscapeCsv(safe_cast<String^>(row->Cells["Details"]->Value));

                sw->WriteLine("{0};{1};{2};{3}", player, eventType, minute, details);
            }

            sw->Close();
        }

        String^ EscapeCsv(String^ value)
        {
            if (String::IsNullOrEmpty(value)) return "";
            if (value->Contains(";") || value->Contains("\"") || value->Contains("\n")) {
                return "\"" + value->Replace("\"", "\"\"") + "\"";
            }
            return value;
        }
    };
}