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

    public ref class MatchEventsBatchForm : public System::Windows::Forms::Form
    {
    public:
        MatchEventsBatchForm(int matchId, String^ matchInfo, String^ connectionString)
        {
            this->matchId = matchId;
            this->matchInfo = matchInfo;
            this->connStr = connectionString;
            InitializeComponent();
            LoadPlayers();
            InitializeEventTypes();
        }

    protected:
        ~MatchEventsBatchForm()
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
        Button^ btnAddRow;
        Button^ btnSave;
        Button^ btnCancel;
        Button^ btnDeleteSelected;

        // Кэш игроков
        DataTable^ playersCache;

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->Size = System::Drawing::Size(900, 600);
            this->StartPosition = FormStartPosition::CenterParent;
            this->Text = "Массовое добавление событий матча";
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
            dataGridEvents->Size = System::Drawing::Size(860, 400);
            dataGridEvents->AllowUserToAddRows = false;
            dataGridEvents->AllowUserToDeleteRows = true;
            dataGridEvents->RowHeadersVisible = true;
            dataGridEvents->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dataGridEvents->MultiSelect = false;

            // Настройка колонок
            InitializeDataGridColumns();

            // Кнопка добавления строки
            btnAddRow = gcnew Button();
            btnAddRow->Text = "➕ Добавить строку";
            btnAddRow->Size = System::Drawing::Size(140, 35);
            btnAddRow->Location = Point(10, 460);
            btnAddRow->BackColor = Color::FromArgb(46, 204, 113);
            btnAddRow->ForeColor = Color::White;
            btnAddRow->FlatStyle = FlatStyle::Flat;
            btnAddRow->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Bold);

            // Кнопка удаления выбранной строки
            btnDeleteSelected = gcnew Button();
            btnDeleteSelected->Text = "Удалить выбранное";
            btnDeleteSelected->Size = System::Drawing::Size(140, 35);
            btnDeleteSelected->Location = Point(160, 460);
            btnDeleteSelected->BackColor = Color::FromArgb(231, 76, 60);
            btnDeleteSelected->ForeColor = Color::White;
            btnDeleteSelected->FlatStyle = FlatStyle::Flat;
            btnDeleteSelected->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Bold);

            // Кнопка сохранения
            btnSave = gcnew Button();
            btnSave->Text = "Сохранить все";
            btnSave->Size = System::Drawing::Size(120, 35);
            btnSave->Location = Point(650, 460);
            btnSave->BackColor = Color::FromArgb(41, 128, 185);
            btnSave->ForeColor = Color::White;
            btnSave->FlatStyle = FlatStyle::Flat;
            btnSave->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Bold);

            // Кнопка отмены
            btnCancel = gcnew Button();
            btnCancel->Text = "Отмена";
            btnCancel->Size = System::Drawing::Size(120, 35);
            btnCancel->Location = Point(780, 460);
            btnCancel->BackColor = Color::FromArgb(108, 117, 125);
            btnCancel->ForeColor = Color::White;
            btnCancel->FlatStyle = FlatStyle::Flat;
            btnCancel->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Bold);

            // Добавление элементов на форму
            this->Controls->Add(lblMatchInfo);
            this->Controls->Add(dataGridEvents);
            this->Controls->Add(btnAddRow);
            this->Controls->Add(btnDeleteSelected);
            this->Controls->Add(btnSave);
            this->Controls->Add(btnCancel);

            // Обработчики событий
            btnAddRow->Click += gcnew EventHandler(this, &MatchEventsBatchForm::btnAddRow_Click);
            btnDeleteSelected->Click += gcnew EventHandler(this, &MatchEventsBatchForm::btnDeleteSelected_Click);
            btnSave->Click += gcnew EventHandler(this, &MatchEventsBatchForm::btnSave_Click);
            btnCancel->Click += gcnew EventHandler(this, &MatchEventsBatchForm::btnCancel_Click);

            // Стилизация DataGridView
            StyleDataGridView();
        }

        void InitializeDataGridColumns()
        {
            dataGridEvents->Columns->Clear();

            // Колонка для выбора игрока
            DataGridViewComboBoxColumn^ playerColumn = gcnew DataGridViewComboBoxColumn();
            playerColumn->HeaderText = "Игрок";
            playerColumn->Name = "Player";
            playerColumn->Width = 150;
            playerColumn->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
            dataGridEvents->Columns->Add(playerColumn);

            // Колонка для типа события
            DataGridViewComboBoxColumn^ eventTypeColumn = gcnew DataGridViewComboBoxColumn();
            eventTypeColumn->HeaderText = "Тип события";
            eventTypeColumn->Name = "EventType";
            eventTypeColumn->Width = 150;
            eventTypeColumn->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
            dataGridEvents->Columns->Add(eventTypeColumn);

            // Колонка для минуты
            DataGridViewTextBoxColumn^ minuteColumn = gcnew DataGridViewTextBoxColumn();
            minuteColumn->HeaderText = "Минута";
            minuteColumn->Name = "minute";
            minuteColumn->Width = 80;
            dataGridEvents->Columns->Add(minuteColumn);

            // Колонка для деталей
            DataGridViewTextBoxColumn^ detailsColumn = gcnew DataGridViewTextBoxColumn();
            detailsColumn->HeaderText = "Детали";
            detailsColumn->Name = "Details";  // ← ИСПРАВЛЕНО: было minuteColumn->Name = "Details";
            detailsColumn->Width = 300;
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
        }

        void LoadPlayers()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                OdbcDataAdapter^ da = gcnew OdbcDataAdapter("SELECT id, \"Имя\" FROM \"Player\" ORDER BY \"Имя\";", conn);
                playersCache = gcnew DataTable();
                da->Fill(playersCache);

                // Заполняем выпадающий список игроков в DataGridView
                DataGridViewComboBoxColumn^ playerColumn = safe_cast<DataGridViewComboBoxColumn^>(dataGridEvents->Columns["Player"]);
                playerColumn->DataSource = playersCache;
                playerColumn->DisplayMember = "Имя";
                playerColumn->ValueMember = "Имя";
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки списка игроков: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void InitializeEventTypes()
        {
            DataGridViewComboBoxColumn^ eventTypeColumn = safe_cast<DataGridViewComboBoxColumn^>(dataGridEvents->Columns["EventType"]);

            array<String^>^ eventTypes = {
                "Гол", "Пасс", "Желтая карточка", "Красная карточка",
                "Замена", "Удар", "Фол", "Угловой", "Штрафной", "Пенальти",
                "Удар по воротам", "Сейв", "Нарушение", "Офсайд"
            };

            eventTypeColumn->Items->AddRange(eventTypes);
        }

        // Обработчики событий
        System::Void btnAddRow_Click(System::Object^ sender, System::EventArgs^ e)
        {
            dataGridEvents->Rows->Add();
        }

        System::Void btnDeleteSelected_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dataGridEvents->CurrentRow != nullptr && !dataGridEvents->CurrentRow->IsNewRow) {
                dataGridEvents->Rows->Remove(dataGridEvents->CurrentRow);
            }
            else {
                MessageBox::Show("Выберите строку для удаления!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
            }
        }

        System::Void btnSave_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dataGridEvents->Rows->Count == 0) {
                MessageBox::Show("Нет данных для сохранения!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            // Проверка данных
            if (!ValidateData()) {
                return;
            }

            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                conn->Open();

                OdbcCommand^ insertCmd = gcnew OdbcCommand(
                    "INSERT INTO matchevents (match_id, player_name, event_type, minute, details) VALUES (?, ?, ?, ?, ?);", conn);
                insertCmd->Parameters->Add(gcnew OdbcParameter("match_id", OdbcType::Int));
                insertCmd->Parameters->Add(gcnew OdbcParameter("player_name", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("event_type", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("minute", OdbcType::Int));
                insertCmd->Parameters->Add(gcnew OdbcParameter("details", OdbcType::Text));

                int savedCount = 0;

                for each (DataGridViewRow ^ row in dataGridEvents->Rows) {
                    if (row->IsNewRow) continue;

                    String^ playerName = safe_cast<String^>(row->Cells["Player"]->Value);
                    String^ eventType = safe_cast<String^>(row->Cells["EventType"]->Value);
                    Object^ minuteObj = row->Cells["minute"]->Value;
                    String^ details = safe_cast<String^>(row->Cells["Details"]->Value);

                    // Пропускаем пустые строки
                    if (String::IsNullOrEmpty(playerName) || String::IsNullOrEmpty(eventType) || minuteObj == nullptr)
                        continue;

                    int minute = Convert::ToInt32(minuteObj);

                    insertCmd->Parameters[0]->Value = matchId;
                    insertCmd->Parameters[1]->Value = playerName;
                    insertCmd->Parameters[2]->Value = eventType;
                    insertCmd->Parameters[3]->Value = minute;
                    insertCmd->Parameters[4]->Value = String::IsNullOrEmpty(details) ? (Object^)DBNull::Value : details;

                    insertCmd->ExecuteNonQuery();
                    savedCount++;
                }

                conn->Close();

                MessageBox::Show(String::Format("Успешно сохранено {0} событий!", savedCount), "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);

                this->DialogResult = Windows::Forms::DialogResult::OK;
                this->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка сохранения событий: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnCancel_Click(System::Object^ sender, System::EventArgs^ e)
        {
            this->DialogResult = Windows::Forms::DialogResult::Cancel;
            this->Close();
        }

        bool ValidateData()
        {
            for each (DataGridViewRow ^ row in dataGridEvents->Rows) {
                if (row->IsNewRow) continue;

                // Проверка игрока
                if (row->Cells["Player"]->Value == nullptr || String::IsNullOrEmpty(safe_cast<String^>(row->Cells["Player"]->Value))) {
                    MessageBox::Show("Не все строки имеют выбранного игрока!", "Ошибка валидации",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return false;
                }

                // Проверка типа события
                if (row->Cells["EventType"]->Value == nullptr || String::IsNullOrEmpty(safe_cast<String^>(row->Cells["EventType"]->Value))) {
                    MessageBox::Show("Не все строки имеют выбранный тип события!", "Ошибка валидации",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return false;
                }

                // Проверка минуты
                if (row->Cells["minute"]->Value == nullptr) {
                    MessageBox::Show("Не все строки имеют указанную минуту!", "Ошибка валидации",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return false;
                }

                // Проверка что минута - число
                try {
                    int minute = Convert::ToInt32(row->Cells["minute"]->Value);
                    if (minute < 1 || minute > 120) {
                        MessageBox::Show("Минута должна быть в диапазоне от 1 до 120!", "Ошибка валидации",
                            MessageBoxButtons::OK, MessageBoxIcon::Error);
                        return false;
                    }
                }
                catch (FormatException^) {
                    MessageBox::Show("Минута должна быть числом!", "Ошибка валидации",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return false;
                }
            }

            return true;
        }
    };
}