#pragma once
#include "LoginForm.h"
#include "TrainingParticipantsForm.h"
#include "MatchEventsViewForm.h"
#include "MatchLineupForm.h"

namespace SUBD {
    using namespace System::Data;
    using namespace System::Data::Odbc;
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
            ApplyModernDesign();
            InitializeSearchPlaceholder();
        }

    protected:
        ~MyForm()
        {
            if (components) delete components;
        }

    private: System::Windows::Forms::DataGridView^ dataGridView1;
    private: System::Windows::Forms::DataGridView^ dataGridView2;
    private: System::Windows::Forms::Button^ btnPlayers;
    private: System::Windows::Forms::Button^ btnStats;
    private: System::Windows::Forms::Button^ btnGoalkeepers;
    private: System::Windows::Forms::Button^ btnAdmin;
    private: System::Windows::Forms::Button^ btnGames;
    private: System::Windows::Forms::Button^ btnMatchEvents;
    private: System::Windows::Forms::Button^ btnLineups;
    private: System::Windows::Forms::Button^ btnInjuries;
    private: System::Windows::Forms::Button^ btnTrainings;
    private: System::Windows::Forms::Panel^ panelHeader;
    private: System::Windows::Forms::Label^ labelTitle;
    private: System::Windows::Forms::Panel^ panelLeft;
    private: System::Windows::Forms::Label^ labelLeftTitle;
    private: System::Windows::Forms::Panel^ panelRight;
    private: System::Windows::Forms::Label^ labelRightTitle;
    private: System::Windows::Forms::Panel^ panelButtons;
    private: System::Windows::Forms::Panel^ panelSearch;
    private: System::Windows::Forms::TextBox^ txtSearch;
    private: System::Windows::Forms::Button^ btnSearch;
    private: System::Windows::Forms::Button^ btnClearSearch;
    private: System::Boolean isPlaceholder;

    private:
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
            this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
            this->btnPlayers = (gcnew System::Windows::Forms::Button());
            this->btnStats = (gcnew System::Windows::Forms::Button());
            this->btnGoalkeepers = (gcnew System::Windows::Forms::Button());
            this->btnAdmin = (gcnew System::Windows::Forms::Button());
            this->btnGames = (gcnew System::Windows::Forms::Button());
            this->btnMatchEvents = (gcnew System::Windows::Forms::Button());
            this->btnLineups = (gcnew System::Windows::Forms::Button());
            this->btnInjuries = (gcnew System::Windows::Forms::Button());
            this->btnTrainings = (gcnew System::Windows::Forms::Button());
            this->panelHeader = (gcnew System::Windows::Forms::Panel());
            this->labelTitle = (gcnew System::Windows::Forms::Label());
            this->panelLeft = (gcnew System::Windows::Forms::Panel());
            this->labelLeftTitle = (gcnew System::Windows::Forms::Label());
            this->panelRight = (gcnew System::Windows::Forms::Panel());
            this->labelRightTitle = (gcnew System::Windows::Forms::Label());
            this->panelButtons = (gcnew System::Windows::Forms::Panel());
            this->panelSearch = (gcnew System::Windows::Forms::Panel());
            this->txtSearch = (gcnew System::Windows::Forms::TextBox());
            this->btnSearch = (gcnew System::Windows::Forms::Button());
            this->btnClearSearch = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView2))->BeginInit();
            this->panelHeader->SuspendLayout();
            this->panelLeft->SuspendLayout();
            this->panelRight->SuspendLayout();
            this->panelButtons->SuspendLayout();
            this->panelSearch->SuspendLayout();
            this->SuspendLayout();

            // dataGridView1 - левая таблица
            this->dataGridView1->AllowUserToAddRows = false;
            this->dataGridView1->AllowUserToDeleteRows = false;
            this->dataGridView1->BackgroundColor = Color::FromArgb(45, 45, 48);
            this->dataGridView1->BorderStyle = BorderStyle::None;
            this->dataGridView1->ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle::Single;
            this->dataGridView1->ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridView1->ColumnHeadersHeight = 40;
            this->dataGridView1->EnableHeadersVisualStyles = false;
            this->dataGridView1->Location = Point(20, 80);
            this->dataGridView1->Name = L"dataGridView1";
            this->dataGridView1->ReadOnly = true;
            this->dataGridView1->RowHeadersVisible = false;
            this->dataGridView1->RowHeadersWidth = 51;
            this->dataGridView1->RowTemplate->Height = 35;
            this->dataGridView1->Size = Drawing::Size(580, 470);
            this->dataGridView1->TabIndex = 0;
            this->dataGridView1->CellClick += gcnew DataGridViewCellEventHandler(this, &MyForm::dataGridView1_CellClick);

            // dataGridView2 - правая таблица
            this->dataGridView2->AllowUserToAddRows = false;
            this->dataGridView2->AllowUserToDeleteRows = false;
            this->dataGridView2->BackgroundColor = Color::FromArgb(45, 45, 48);
            this->dataGridView2->BorderStyle = BorderStyle::None;
            this->dataGridView2->ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle::Single;
            this->dataGridView2->ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridView2->ColumnHeadersHeight = 40;
            this->dataGridView2->EnableHeadersVisualStyles = false;
            this->dataGridView2->Location = Point(20, 80);
            this->dataGridView2->Name = L"dataGridView2";
            this->dataGridView2->ReadOnly = true;
            this->dataGridView2->RowHeadersVisible = false;
            this->dataGridView2->RowHeadersWidth = 51;
            this->dataGridView2->RowTemplate->Height = 35;
            this->dataGridView2->Size = Drawing::Size(580, 470);
            this->dataGridView2->TabIndex = 4;
            this->dataGridView2->CellDoubleClick += gcnew DataGridViewCellEventHandler(this, &MyForm::dataGridView2_CellDoubleClick);

            // Панель поиска
            this->panelSearch->BackColor = Color::FromArgb(37, 37, 38);
            this->panelSearch->Controls->Add(this->txtSearch);
            this->panelSearch->Controls->Add(this->btnSearch);
            this->panelSearch->Controls->Add(this->btnClearSearch);
            this->panelSearch->Location = Point(20, 180);
            this->panelSearch->Name = L"panelSearch";
            this->panelSearch->Size = Drawing::Size(1240, 40);
            this->panelSearch->TabIndex = 9;

            // Текстовое поле поиска
            this->txtSearch->BackColor = Color::FromArgb(45, 45, 48);
            this->txtSearch->ForeColor = Color::Gray;
            this->txtSearch->Location = Point(10, 8);
            this->txtSearch->Name = L"txtSearch";
            this->txtSearch->Size = Drawing::Size(900, 27);
            this->txtSearch->TabIndex = 0;
            this->txtSearch->Font = gcnew Drawing::Font(L"Segoe UI", 9);
            this->txtSearch->Text = L"🔍 Поиск по всем данным (фамилия, команда, позиция и т.д.)...";
            this->txtSearch->GotFocus += gcnew EventHandler(this, &MyForm::txtSearch_GotFocus);
            this->txtSearch->LostFocus += gcnew EventHandler(this, &MyForm::txtSearch_LostFocus);
            this->txtSearch->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::txtSearch_KeyPress);

            // Кнопка поиска
            this->btnSearch->BackColor = Color::FromArgb(0, 122, 204);
            this->btnSearch->FlatStyle = FlatStyle::Flat;
            this->btnSearch->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
            this->btnSearch->ForeColor = Color::White;
            this->btnSearch->Location = Point(920, 8);
            this->btnSearch->Name = L"btnSearch";
            this->btnSearch->Size = Drawing::Size(100, 25);
            this->btnSearch->TabIndex = 1;
            this->btnSearch->Text = L"Найти";
            this->btnSearch->UseVisualStyleBackColor = false;
            this->btnSearch->FlatAppearance->BorderColor = Color::FromArgb(0, 122, 204);
            this->btnSearch->Click += gcnew EventHandler(this, &MyForm::btnSearch_Click);

            // Кнопка очистки поиска
            this->btnClearSearch->BackColor = Color::FromArgb(63, 63, 70);
            this->btnClearSearch->FlatStyle = FlatStyle::Flat;
            this->btnClearSearch->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
            this->btnClearSearch->ForeColor = Color::White;
            this->btnClearSearch->Location = Point(1030, 8);
            this->btnClearSearch->Name = L"btnClearSearch";
            this->btnClearSearch->Size = Drawing::Size(100, 25);
            this->btnClearSearch->TabIndex = 2;
            this->btnClearSearch->Text = L"Очистить";
            this->btnClearSearch->UseVisualStyleBackColor = false;
            this->btnClearSearch->FlatAppearance->BorderColor = Color::FromArgb(63, 63, 70);
            this->btnClearSearch->Click += gcnew EventHandler(this, &MyForm::btnClearSearch_Click);

            // Панель для кнопок
            this->panelButtons->BackColor = Color::FromArgb(37, 37, 38);
            this->panelButtons->Controls->Add(this->btnTrainings);
            this->panelButtons->Controls->Add(this->btnInjuries);
            this->panelButtons->Controls->Add(this->btnLineups);
            this->panelButtons->Controls->Add(this->btnMatchEvents);
            this->panelButtons->Controls->Add(this->btnGames);
            this->panelButtons->Controls->Add(this->btnAdmin);
            this->panelButtons->Controls->Add(this->btnGoalkeepers);
            this->panelButtons->Controls->Add(this->btnStats);
            this->panelButtons->Controls->Add(this->btnPlayers);
            this->panelButtons->Location = Point(20, 100);
            this->panelButtons->Name = L"panelButtons";
            this->panelButtons->Size = Drawing::Size(1240, 60);
            this->panelButtons->TabIndex = 8;

            // Кнопки
            InitializeButton(btnPlayers, L"Игроки", Point(20, 10), 100);
            InitializeButton(btnStats, L"Статистика", Point(130, 10), 100);
            InitializeButton(btnGoalkeepers, L"Вратари", Point(240, 10), 100);
            InitializeButton(btnGames, L"Матчи", Point(350, 10), 100);
            InitializeButton(btnMatchEvents, L"События", Point(460, 10), 100);
            InitializeButton(btnLineups, L"Составы", Point(570, 10), 100);
            InitializeButton(btnInjuries, L"Травмы", Point(680, 10), 100);
            InitializeButton(btnTrainings, L"Тренировки", Point(790, 10), 100);
            InitializeButton(btnAdmin, L"Админ", Point(900, 10), 100);

            // Заголовок
            this->panelHeader->BackColor = Color::FromArgb(0, 122, 204);
            this->panelHeader->Controls->Add(this->labelTitle);
            this->panelHeader->Dock = DockStyle::Top;
            this->panelHeader->Location = Point(0, 0);
            this->panelHeader->Name = L"panelHeader";
            this->panelHeader->Size = Drawing::Size(1280, 80);
            this->panelHeader->TabIndex = 5;

            this->labelTitle->AutoSize = true;
            this->labelTitle->Font = gcnew Drawing::Font(L"Segoe UI", 16, FontStyle::Bold);
            this->labelTitle->ForeColor = Color::White;
            this->labelTitle->Location = Point(20, 25);
            this->labelTitle->Name = L"labelTitle";
            this->labelTitle->Size = Drawing::Size(415, 37);
            this->labelTitle->TabIndex = 0;
            this->labelTitle->Text = L"⚽ Футбольная команда (PostgreSQL)";

            // Левая панель
            this->panelLeft->BackColor = Color::FromArgb(37, 37, 38);
            this->panelLeft->Controls->Add(this->labelLeftTitle);
            this->panelLeft->Controls->Add(this->dataGridView1);
            this->panelLeft->Location = Point(20, 240);
            this->panelLeft->Name = L"panelLeft";
            this->panelLeft->Size = Drawing::Size(600, 570);
            this->panelLeft->TabIndex = 6;

            this->labelLeftTitle->AutoSize = true;
            this->labelLeftTitle->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            this->labelLeftTitle->ForeColor = Color::FromArgb(0, 122, 204);
            this->labelLeftTitle->Location = Point(20, 15);
            this->labelLeftTitle->Name = L"labelLeftTitle";
            this->labelLeftTitle->Size = Drawing::Size(142, 23);
            this->labelLeftTitle->TabIndex = 1;
            this->labelLeftTitle->Text = L"Список игроков";

            // Правая панель
            this->panelRight->BackColor = Color::FromArgb(37, 37, 38);
            this->panelRight->Controls->Add(this->labelRightTitle);
            this->panelRight->Controls->Add(this->dataGridView2);
            this->panelRight->Location = Point(640, 240);
            this->panelRight->Name = L"panelRight";
            this->panelRight->Size = Drawing::Size(620, 570);
            this->panelRight->TabIndex = 7;

            this->labelRightTitle->AutoSize = true;
            this->labelRightTitle->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            this->labelRightTitle->ForeColor = Color::FromArgb(0, 122, 204);
            this->labelRightTitle->Location = Point(20, 15);
            this->labelRightTitle->Name = L"labelRightTitle";
            this->labelRightTitle->Size = Drawing::Size(195, 23);
            this->labelRightTitle->TabIndex = 1;
            this->labelRightTitle->Text = L"Детальная информация";

            // Основная форма
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = Color::FromArgb(30, 30, 30);
            this->ClientSize = Drawing::Size(1280, 840);
            this->Controls->Add(this->panelSearch);
            this->Controls->Add(this->panelButtons);
            this->Controls->Add(this->panelRight);
            this->Controls->Add(this->panelLeft);
            this->Controls->Add(this->panelHeader);
            this->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Regular, GraphicsUnit::Point, static_cast<Byte>(204));
            this->ForeColor = Color::White;
            this->MinimumSize = Drawing::Size(1300, 880);
            this->Name = L"MyForm";
            this->StartPosition = FormStartPosition::CenterScreen;
            this->Text = L"Футбольная команда (PostgreSQL)";
            this->Load += gcnew EventHandler(this, &MyForm::MyForm_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView2))->EndInit();
            this->panelHeader->ResumeLayout(false);
            this->panelHeader->PerformLayout();
            this->panelLeft->ResumeLayout(false);
            this->panelLeft->PerformLayout();
            this->panelRight->ResumeLayout(false);
            this->panelRight->PerformLayout();
            this->panelButtons->ResumeLayout(false);
            this->panelSearch->ResumeLayout(false);
            this->panelSearch->PerformLayout();
            this->ResumeLayout(false);
        }
#pragma endregion

    private:
        void InitializeButton(Button^ button, String^ text, Point location, int width)
        {
            button->BackColor = Color::FromArgb(63, 63, 70);
            button->FlatStyle = FlatStyle::Flat;
            button->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
            button->ForeColor = Color::White;
            button->Location = location;
            button->Name = text->Replace(" ", "")->ToLower();
            button->Size = Drawing::Size(width, 40);
            button->TabIndex = 1;
            button->Text = text;
            button->UseVisualStyleBackColor = false;
            button->FlatAppearance->BorderColor = Color::FromArgb(0, 122, 204);
            button->FlatAppearance->BorderSize = 1;
            button->FlatAppearance->MouseOverBackColor = Color::FromArgb(0, 122, 204);

            // Назначаем обработчики событий
            if (text == L"Игроки") {
                button->Click += gcnew EventHandler(this, &MyForm::btnPlayers_Click);
            }
            else if (text == L"Статистика") {
                button->Click += gcnew EventHandler(this, &MyForm::btnStats_Click);
            }
            else if (text == L"Вратари") {
                button->Click += gcnew EventHandler(this, &MyForm::btnGoalkeepers_Click);
            }
            else if (text == L"Матчи") {
                button->Click += gcnew EventHandler(this, &MyForm::btnGames_Click);
            }
            else if (text == L"События") {
                button->Click += gcnew EventHandler(this, &MyForm::btnMatchEvents_Click);
            }
            else if (text == L"Составы") {
                button->Click += gcnew EventHandler(this, &MyForm::btnLineups_Click);
            }
            else if (text == L"Травмы") {
                button->Click += gcnew EventHandler(this, &MyForm::btnInjuries_Click);
            }
            else if (text == L"Тренировки") {
                button->Click += gcnew EventHandler(this, &MyForm::btnTrainings_Click);
            }
            else if (text == L"Админ") {
                button->Click += gcnew EventHandler(this, &MyForm::btnAdmin_Click);
            }
        }

        void ApplyModernDesign()
        {
            // Настройка внешнего вида DataGridView
            ApplyGridViewStyle(dataGridView1);
            ApplyGridViewStyle(dataGridView2);
        }

        void ApplyGridViewStyle(DataGridView^ grid)
        {
            grid->BackgroundColor = Color::FromArgb(45, 45, 48);
            grid->BorderStyle = BorderStyle::None;
            grid->EnableHeadersVisualStyles = false;

            // Стиль заголовков колонок
            grid->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(0, 122, 204);
            grid->ColumnHeadersDefaultCellStyle->ForeColor = Color::White;
            grid->ColumnHeadersDefaultCellStyle->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            grid->ColumnHeadersDefaultCellStyle->Alignment = DataGridViewContentAlignment::MiddleCenter;
            grid->ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle::Single;
            grid->ColumnHeadersHeight = 40;

            // Стиль строк
            grid->DefaultCellStyle->BackColor = Color::FromArgb(45, 45, 48);
            grid->DefaultCellStyle->ForeColor = Color::White;
            grid->DefaultCellStyle->Font = gcnew Drawing::Font(L"Segoe UI", 9);
            grid->DefaultCellStyle->SelectionBackColor = Color::FromArgb(0, 122, 204);
            grid->DefaultCellStyle->SelectionForeColor = Color::White;

            // Альтернативный цвет строк
            grid->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(55, 55, 58);
            grid->AlternatingRowsDefaultCellStyle->ForeColor = Color::White;

            // Отключение стандартных заголовков строк
            grid->RowHeadersVisible = false;

            // Выравнивание текста
            grid->DefaultCellStyle->Alignment = DataGridViewContentAlignment::MiddleLeft;

            // Высота строк
            grid->RowTemplate->Height = 35;

            // Настройки отображения
            grid->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            grid->AllowUserToResizeRows = false;
            grid->AllowUserToAddRows = false;
            grid->AllowUserToDeleteRows = false;
            grid->ReadOnly = true;
            grid->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            grid->MultiSelect = false;
        }

   private: String^ connStr = "DSN=AliansDB;UID=readonly_user;PWD=readonly123;";

    private: void LoadTable(String^ query, DataGridView^ grid) {
        OdbcConnection^ conn = gcnew OdbcConnection(connStr);
        try {
            conn->Open();
            OdbcDataAdapter^ da = gcnew OdbcDataAdapter(query, conn);
            DataTable^ dt = gcnew DataTable();
            da->Fill(dt);
            grid->DataSource = dt;

            // Автоматическая настройка ширины колонок после загрузки данных
            if (grid->Columns->Count > 0) {
                grid->AutoResizeColumns(DataGridViewAutoSizeColumnsMode::AllCells);
            }

            conn->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка: " + ex->Message, "Ошибка базы данных",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
        LoadTable("SELECT id, Имя AS \"Имя\", Позиция AS \"Позиция\" FROM \"Player\";", dataGridView1);
        labelLeftTitle->Text = L"👥 Список игроков";
        labelRightTitle->Text = L"ℹ️ Детальная информация";
        ResetButtonColors();
        btnPlayers->BackColor = Color::FromArgb(0, 122, 204);
    }

    private: System::Void btnPlayers_Click(System::Object^ sender, System::EventArgs^ e) {
        LoadTable("SELECT id, Имя AS \"Имя\", Позиция AS \"Позиция\" FROM \"Player\";", dataGridView1);
        labelLeftTitle->Text = L"👥 Список игроков";
        labelRightTitle->Text = L"ℹ️ Детальная информация";
        ResetButtonColors();
        btnPlayers->BackColor = Color::FromArgb(0, 122, 204);
    }

    private: System::Void btnStats_Click(System::Object^ sender, System::EventArgs^ e) {
        LoadTable(
            "SELECT p.\"Имя\" AS \"Игрок\", s.\"матчи\", s.\"голы\", s.\"голевые передачи\", (s.\"голы\" + s.\"голевые передачи\") AS \"г+п\" "
            "FROM \"Statistika\" s JOIN \"Player\" p ON s.id = p.id;",
            dataGridView2
        );
        labelRightTitle->Text = L"📊 Статистика игроков";
        ResetButtonColors();
        btnStats->BackColor = Color::FromArgb(0, 122, 204);
    }

    private: System::Void btnGoalkeepers_Click(System::Object^ sender, System::EventArgs^ e) {
        LoadTable(
            "SELECT p.\"Имя\" AS \"Игрок\", g.\"пропущенные мячи\" "
            "FROM \"Player\" p JOIN \"Statgp\" g ON p.id = g.id;",
            dataGridView2
        );
        labelRightTitle->Text = L"🥅 Статистика вратарей";
        ResetButtonColors();
        btnGoalkeepers->BackColor = Color::FromArgb(0, 122, 204);
    }

    private: System::Void btnGames_Click(System::Object^ sender, System::EventArgs^ e) {
        LoadTable("SELECT id, date AS \"Дата\", hause AS \"Хозяева\", gast AS \"Гости\", goalhause AS \"Голы хозяев\", goalgast AS \"Голы гостей\" FROM \"igri\";", dataGridView2);
        labelRightTitle->Text = L"📅 Расписание матчей";
        ResetButtonColors();
        btnGames->BackColor = Color::FromArgb(0, 122, 204);
    }

    private: System::Void btnMatchEvents_Click(System::Object^ sender, System::EventArgs^ e) {
        LoadTable(
            "SELECT e.event_id AS \"ID\", "
            "CONCAT(m.hause, ' vs ', m.gast, ' (', TO_CHAR(m.date, 'DD.MM.YYYY'), ')') AS \"Матч\", "
            "e.player_name AS \"Игрок\", "
            "e.event_type AS \"Событие\", e.minute AS \"Минута\", e.details AS \"Детали\" "
            "FROM \"matchevents\" e "
            "JOIN \"igri\" m ON e.match_id = m.id "
            "ORDER BY m.date, e.minute;",
            dataGridView2
        );
        labelRightTitle->Text = L"⚡ События матчей";
        ResetButtonColors();
        btnMatchEvents->BackColor = Color::FromArgb(0, 122, 204);
    }

    private: System::Void btnLineups_Click(System::Object^ sender, System::EventArgs^ e) {
        LoadTable(
            "SELECT l.lineup_id AS \"ID\", "
            "CONCAT(m.hause, ' vs ', m.gast, ' (', TO_CHAR(m.date, 'DD.MM.YYYY'), ')') AS \"Матч\", "
            "l.player_name AS \"Игрок\", "
            "l.position AS \"Позиция\", "
            "CASE WHEN l.is_starter THEN 'Основной' ELSE 'Запасной' END AS \"Статус\" "
            "FROM \"matchlineups\" l "
            "JOIN \"igri\" m ON l.match_id = m.id "
            "ORDER BY m.date, l.is_starter DESC;",
            dataGridView2
        );
        labelRightTitle->Text = L"👥 Составы на матчи";
        ResetButtonColors();
        btnLineups->BackColor = Color::FromArgb(0, 122, 204);
    }

    private: System::Void btnInjuries_Click(System::Object^ sender, System::EventArgs^ e) {
        LoadTable(
            "SELECT injury_id AS \"ID\", player_name AS \"Игрок\", injury_type AS \"Тип травмы\", "
            "date_start AS \"Начало\", date_end AS \"Конец\", status AS \"Статус\" "
            "FROM \"injuries\" ORDER BY date_start DESC;",
            dataGridView2
        );
        labelRightTitle->Text = L"🏥 Травмы игроков";
        ResetButtonColors();
        btnInjuries->BackColor = Color::FromArgb(0, 122, 204);
    }

    private: System::Void btnTrainings_Click(System::Object^ sender, System::EventArgs^ e) {
        LoadTable(
            "SELECT training_id AS \"ID\", training_date AS \"Дата\", "
            "focus_area AS \"Фокус тренировки\", participants AS \"Участники\" "
            "FROM \"trainings\" ORDER BY training_date DESC;",
            dataGridView2
        );

        // Ограничиваем ширину столбца "Участники" после загрузки данных
        if (dataGridView2->Columns->Count > 0 && dataGridView2->Columns["Участники"] != nullptr) {
            dataGridView2->Columns["Участники"]->Width = 200;
            dataGridView2->Columns["Участники"]->DefaultCellStyle->WrapMode = DataGridViewTriState::True;
        }

        labelRightTitle->Text = L"💪 Тренировки";
        ResetButtonColors();
        btnTrainings->BackColor = Color::FromArgb(0, 122, 204);
    }

    private: System::Void dataGridView1_CellClick(System::Object^ sender, DataGridViewCellEventArgs^ e) {
        if (e->RowIndex < 0) return;

        int playerId = Convert::ToInt32(dataGridView1->Rows[e->RowIndex]->Cells["id"]->Value);
        String^ playerName = dataGridView1->Rows[e->RowIndex]->Cells["Имя"]->Value->ToString();
        String^ position = dataGridView1->Rows[e->RowIndex]->Cells["Позиция"]->Value->ToString();

        OdbcConnection^ conn = gcnew OdbcConnection(connStr);
        conn->Open();

        String^ query;
        if (position == "Вратарь") {
            query =
                "SELECT p.\"Имя\" AS \"Игрок\", g.\"пропущенные мячи\" AS \"Пропущенные мячи\" "
                "FROM \"Statgp\" g "
                "JOIN \"Player\" p ON g.id = p.id "
                "WHERE g.id = " + playerId + ";";
            labelRightTitle->Text = L"🥅 Статистика вратаря: " + playerName;
        }
        else {
            query =
                "SELECT p.\"Имя\" AS \"Игрок\", s.\"матчи\" AS \"Матчи\", s.\"голы\" AS \"Голы\", "
                "s.\"голевые передачи\" AS \"Передачи\", (s.\"голы\" + s.\"голевые передачи\") AS \"Гол+Пас\" "
                "FROM \"Statistika\" s "
                "JOIN \"Player\" p ON s.id = p.id "
                "WHERE s.id = " + playerId + ";";
            labelRightTitle->Text = L"📊 Статистика игрока: " + playerName;
        }

        OdbcDataAdapter^ da = gcnew OdbcDataAdapter(query, conn);
        DataTable^ dt = gcnew DataTable();
        da->Fill(dt);
        dataGridView2->DataSource = dt;

        conn->Close();
    }

    private: System::Void dataGridView2_CellDoubleClick(System::Object^ sender, DataGridViewCellEventArgs^ e) {
        if (e->RowIndex < 0) return;

        // Проверяем, что открыта вкладка тренировок
        if (labelRightTitle->Text == L"💪 Тренировки") {
            // Получаем данные о тренировке
            DataGridViewRow^ row = dataGridView2->Rows[e->RowIndex];
            int trainingId = Convert::ToInt32(row->Cells["ID"]->Value);
            String^ trainingDate = row->Cells["Дата"]->Value->ToString();
            String^ focusArea = row->Cells["Фокус тренировки"]->Value->ToString();

            String^ trainingInfo = trainingDate + " - " + focusArea;

            // Открываем форму с участниками тренировки
            TrainingParticipantsForm^ participantsForm = gcnew TrainingParticipantsForm(trainingId, trainingInfo, connStr);
            participantsForm->ShowDialog();
        }
        // Проверяем, что открыта вкладка событий матчей
        else if (labelRightTitle->Text == L"⚡ События матчей") {
            // Получаем данные о матче
            DataGridViewRow^ row = dataGridView2->Rows[e->RowIndex];

            // Получаем ID события и информацию о матче
            int eventId = Convert::ToInt32(row->Cells["ID"]->Value);
            String^ matchInfo = row->Cells["Матч"]->Value->ToString();

            // Получаем ID матча из базы данных
            int matchId = GetMatchIdFromEvent(eventId);

            if (matchId > 0) {
                // Открываем форму с детальными событиями матча
                MatchEventsViewForm^ eventsForm = gcnew MatchEventsViewForm(matchId, matchInfo, connStr);
                eventsForm->ShowDialog();
            }
        }
        // Проверяем, что открыта вкладка составов на матчи
        else if (labelRightTitle->Text == L"👥 Составы на матчи") {
            // Получаем данные о составе
            DataGridViewRow^ row = dataGridView2->Rows[e->RowIndex];

            // Получаем ID состава и информацию о матче
            int lineupId = Convert::ToInt32(row->Cells["ID"]->Value);
            String^ matchInfo = row->Cells["Матч"]->Value->ToString();

            // Получаем ID матча из базы данных
            int matchId = GetMatchIdFromLineup(lineupId);

            if (matchId > 0) {
                // Открываем форму с составом на матч
                MatchLineupForm^ lineupForm = gcnew MatchLineupForm(matchId, matchInfo, connStr);
                lineupForm->ShowDialog();
            }
        }
    }

    private: int GetMatchIdFromEvent(int eventId)
    {
        try {
            OdbcConnection^ conn = gcnew OdbcConnection(connStr);
            OdbcCommand^ cmd = gcnew OdbcCommand("SELECT match_id FROM matchevents WHERE event_id = ?", conn);
            cmd->Parameters->AddWithValue("event_id", eventId);

            conn->Open();
            Object^ result = cmd->ExecuteScalar();
            conn->Close();

            if (result != nullptr && result != DBNull::Value) {
                return Convert::ToInt32(result);
            }
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка получения ID матча: " + ex->Message, "Ошибка",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
        return -1;
    }

    private: int GetMatchIdFromLineup(int lineupId)
    {
        try {
            OdbcConnection^ conn = gcnew OdbcConnection(connStr);
            OdbcCommand^ cmd = gcnew OdbcCommand("SELECT match_id FROM matchlineups WHERE lineup_id = ?", conn);
            cmd->Parameters->AddWithValue("lineup_id", lineupId);

            conn->Open();
            Object^ result = cmd->ExecuteScalar();
            conn->Close();

            if (result != nullptr && result != DBNull::Value) {
                return Convert::ToInt32(result);
            }
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка получения ID матча: " + ex->Message, "Ошибка",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
        return -1;
    }

    private: System::Void btnAdmin_Click(System::Object^ sender, System::EventArgs^ e) {
        ResetButtonColors();
        btnAdmin->BackColor = Color::FromArgb(0, 122, 204);
        LoginForm^ loginForm = gcnew LoginForm();
        loginForm->ShowDialog();
    }

    private: void ResetButtonColors()
    {
        btnPlayers->BackColor = Color::FromArgb(63, 63, 70);
        btnStats->BackColor = Color::FromArgb(63, 63, 70);
        btnGoalkeepers->BackColor = Color::FromArgb(63, 63, 70);
        btnGames->BackColor = Color::FromArgb(63, 63, 70);
        btnMatchEvents->BackColor = Color::FromArgb(63, 63, 70);
        btnLineups->BackColor = Color::FromArgb(63, 63, 70);
        btnInjuries->BackColor = Color::FromArgb(63, 63, 70);
        btnTrainings->BackColor = Color::FromArgb(63, 63, 70);
        btnAdmin->BackColor = Color::FromArgb(63, 63, 70);
    }

    private: void InitializeSearchPlaceholder()
    {
        isPlaceholder = true;
        txtSearch->ForeColor = Color::Gray;
    }

    private: System::Void txtSearch_GotFocus(System::Object^ sender, System::EventArgs^ e)
    {
        if (isPlaceholder)
        {
            txtSearch->Text = "";
            txtSearch->ForeColor = Color::White;
            isPlaceholder = false;
        }
    }

    private: System::Void txtSearch_LostFocus(System::Object^ sender, System::EventArgs^ e)
    {
        if (String::IsNullOrWhiteSpace(txtSearch->Text))
        {
            txtSearch->Text = L"🔍 Поиск по всем данным (фамилия, команда, позиция и т.д.)...";
            txtSearch->ForeColor = Color::Gray;
            isPlaceholder = true;
        }
    }

    private: System::Void btnSearch_Click(System::Object^ sender, System::EventArgs^ e) {
        PerformSearch();
    }

    private: System::Void btnClearSearch_Click(System::Object^ sender, System::EventArgs^ e) {
        txtSearch->Text = "";
        isPlaceholder = true;
        txtSearch->ForeColor = Color::Gray;
        txtSearch->Text = L"🔍 Поиск по всем данным (фамилия, команда, позиция и т.д.)...";

        // Восстанавливаем отображение по умолчанию
        if (btnPlayers->BackColor == Color::FromArgb(0, 122, 204)) {
            btnPlayers_Click(sender, e);
        }
        else if (btnStats->BackColor == Color::FromArgb(0, 122, 204)) {
            btnStats_Click(sender, e);
        }
        else if (btnGoalkeepers->BackColor == Color::FromArgb(0, 122, 204)) {
            btnGoalkeepers_Click(sender, e);
        }
        else if (btnGames->BackColor == Color::FromArgb(0, 122, 204)) {
            btnGames_Click(sender, e);
        }
        else if (btnMatchEvents->BackColor == Color::FromArgb(0, 122, 204)) {
            btnMatchEvents_Click(sender, e);
        }
        else if (btnLineups->BackColor == Color::FromArgb(0, 122, 204)) {
            btnLineups_Click(sender, e);
        }
        else if (btnInjuries->BackColor == Color::FromArgb(0, 122, 204)) {
            btnInjuries_Click(sender, e);
        }
        else if (btnTrainings->BackColor == Color::FromArgb(0, 122, 204)) {
            btnTrainings_Click(sender, e);
        }
        else {
            btnPlayers_Click(sender, e);
        }
    }

    private: System::Void txtSearch_KeyPress(System::Object^ sender, KeyPressEventArgs^ e) {
        // Поиск при нажатии Enter
        if (e->KeyChar == (char)Keys::Enter) {
            PerformSearch();
            e->Handled = true; // Предотвращаем звуковой сигнал
        }
    }

    private: void PerformSearch()
    {
        // Проверяем, не placeholder ли это
        if (isPlaceholder)
        {
            MessageBox::Show("Введите текст для поиска", "Поиск",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
            return;
        }

        String^ searchTerm = txtSearch->Text->Trim();

        if (String::IsNullOrEmpty(searchTerm)) {
            MessageBox::Show("Введите текст для поиска", "Поиск",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
            return;
        }

        try {
            OdbcConnection^ conn = gcnew OdbcConnection(connStr);
            conn->Open();

            // Основной запрос поиска по всем таблицам
            String^ query =
                "SELECT 'Игрок' AS \"Тип\", " +
                "p.\"Имя\" AS \"Название\", " +
                "p.\"Позиция\" AS \"Описание\", " +
                "CAST(p.id AS VARCHAR) AS \"ID\", " +
                "NULL AS \"Дополнительно\" " +
                "FROM \"Player\" p " +
                "WHERE UPPER(p.\"Имя\") LIKE UPPER('%" + searchTerm + "%') " +
                "OR UPPER(p.\"Позиция\") LIKE UPPER('%" + searchTerm + "%') " +
                "UNION ALL " +
                "SELECT 'Статистика' AS \"Тип\", " +
                "p.\"Имя\" AS \"Название\", " +
                "CONCAT('Голы: ', s.\"голы\", ', Передачи: ', s.\"голевые передачи\") AS \"Описание\", " +
                "CAST(s.id AS VARCHAR) AS \"ID\", " +
                "CONCAT('Матчи: ', s.\"матчи\") AS \"Дополнительно\" " +
                "FROM \"Statistika\" s " +
                "JOIN \"Player\" p ON s.id = p.id " +
                "WHERE UPPER(p.\"Имя\") LIKE UPPER('%" + searchTerm + "%') " +
                "OR CAST(s.\"голы\" AS VARCHAR) LIKE '%" + searchTerm + "%' " +
                "OR CAST(s.\"голевые передачи\" AS VARCHAR) LIKE '%" + searchTerm + "%' " +
                "UNION ALL " +
                "SELECT 'Статистика вратаря' AS \"Тип\", " +
                "p.\"Имя\" AS \"Название\", " +
                "CONCAT('Пропущенные мячи: ', g.\"пропущенные мячи\") AS \"Описание\", " +
                "CAST(g.id AS VARCHAR) AS \"ID\", " +
                "'Вратарь' AS \"Дополнительно\" " +
                "FROM \"Statgp\" g " +
                "JOIN \"Player\" p ON g.id = p.id " +
                "WHERE UPPER(p.\"Имя\") LIKE UPPER('%" + searchTerm + "%') " +
                "OR CAST(g.\"пропущенные мячи\" AS VARCHAR) LIKE '%" + searchTerm + "%' " +
                "UNION ALL " +
                "SELECT 'Матч' AS \"Тип\", " +
                "CONCAT(i.hause, ' vs ', i.gast) AS \"Название\", " +
                "CONCAT('Дата: ', TO_CHAR(i.date, 'DD.MM.YYYY'), ', Счет: ', i.goalhause, '-', i.goalgast) AS \"Описание\", " +
                "CAST(i.id AS VARCHAR) AS \"ID\", " +
                "CONCAT('Хозяева: ', i.hause, ', Гости: ', i.gast) AS \"Дополнительно\" " +
                "FROM \"igri\" i " +
                "WHERE UPPER(i.hause) LIKE UPPER('%" + searchTerm + "%') " +
                "OR UPPER(i.gast) LIKE UPPER('%" + searchTerm + "%') " +
                "OR TO_CHAR(i.date, 'DD.MM.YYYY') LIKE '%" + searchTerm + "%' " +
                "OR CAST(i.goalhause AS VARCHAR) LIKE '%" + searchTerm + "%' " +
                "OR CAST(i.goalgast AS VARCHAR) LIKE '%" + searchTerm + "%' " +
                "UNION ALL " +
                "SELECT 'Событие матча' AS \"Тип\", " +
                "e.player_name AS \"Название\", " +
                "CONCAT('Событие: ', e.event_type, ', Минута: ', e.minute) AS \"Описание\", " +
                "CAST(e.event_id AS VARCHAR) AS \"ID\", " +
                "CONCAT('Матч: ', m.hause, ' vs ', m.gast, ' (', TO_CHAR(m.date, 'DD.MM.YYYY'), ')') AS \"Дополнительно\" " +
                "FROM \"matchevents\" e " +
                "JOIN \"igri\" m ON e.match_id = m.id " +
                "WHERE UPPER(e.player_name) LIKE UPPER('%" + searchTerm + "%') " +
                "OR UPPER(e.event_type) LIKE UPPER('%" + searchTerm + "%') " +
                "OR UPPER(e.details) LIKE UPPER('%" + searchTerm + "%') " +
                "OR CAST(e.minute AS VARCHAR) LIKE '%" + searchTerm + "%' " +
                "UNION ALL " +
                "SELECT 'Состав на матч' AS \"Тип\", " +
                "l.player_name AS \"Название\", " +
                "CONCAT('Позиция: ', l.position, ', Статус: ', CASE WHEN l.is_starter THEN 'Основной' ELSE 'Запасной' END) AS \"Описание\", " +
                "CAST(l.lineup_id AS VARCHAR) AS \"ID\", " +
                "CONCAT('Матч: ', m.hause, ' vs ', m.gast, ' (', TO_CHAR(m.date, 'DD.MM.YYYY'), ')') AS \"Дополнительно\" " +
                "FROM \"matchlineups\" l " +
                "JOIN \"igri\" m ON l.match_id = m.id " +
                "WHERE UPPER(l.player_name) LIKE UPPER('%" + searchTerm + "%') " +
                "OR UPPER(l.position) LIKE UPPER('%" + searchTerm + "%') " +
                "UNION ALL " +
                "SELECT 'Травма' AS \"Тип\", " +
                "i.player_name AS \"Название\", " +
                "CONCAT('Тип: ', i.injury_type, ', Статус: ', i.status) AS \"Описание\", " +
                "CAST(i.injury_id AS VARCHAR) AS \"ID\", " +
                "CONCAT('Период: ', TO_CHAR(i.date_start, 'DD.MM.YYYY'), ' - ', TO_CHAR(i.date_end, 'DD.MM.YYYY')) AS \"Дополнительно\" " +
                "FROM \"injuries\" i " +
                "WHERE UPPER(i.player_name) LIKE UPPER('%" + searchTerm + "%') " +
                "OR UPPER(i.injury_type) LIKE UPPER('%" + searchTerm + "%') " +
                "OR UPPER(i.status) LIKE UPPER('%" + searchTerm + "%') " +
                "OR TO_CHAR(i.date_start, 'DD.MM.YYYY') LIKE '%" + searchTerm + "%' " +
                "OR TO_CHAR(i.date_end, 'DD.MM.YYYY') LIKE '%" + searchTerm + "%' " +
                "UNION ALL " +
                "SELECT 'Тренировка' AS \"Тип\", " +
                "t.focus_area AS \"Название\", " +
                "CONCAT('Дата: ', TO_CHAR(t.training_date, 'DD.MM.YYYY'), ', Участники: ', LEFT(t.participants, 50)) AS \"Описание\", " +
                "CAST(t.training_id AS VARCHAR) AS \"ID\", " +
                "NULL AS \"Дополнительно\" " +
                "FROM \"trainings\" t " +
                "WHERE UPPER(t.focus_area) LIKE UPPER('%" + searchTerm + "%') " +
                "OR UPPER(t.participants) LIKE UPPER('%" + searchTerm + "%') " +
                "OR TO_CHAR(t.training_date, 'DD.MM.YYYY') LIKE '%" + searchTerm + "%' " +
                "ORDER BY \"Тип\", \"Название\";";

            OdbcDataAdapter^ da = gcnew OdbcDataAdapter(query, conn);
            DataTable^ dt = gcnew DataTable();
            da->Fill(dt);

            // Отображаем результаты в обеих таблицах
            dataGridView1->DataSource = dt;
            dataGridView2->DataSource = nullptr;

            // Настраиваем заголовки
            labelLeftTitle->Text = L"🔍 Результаты поиска: " + searchTerm;
            labelRightTitle->Text = L"ℹ️ Детальная информация";

            // Очищаем выделение кнопок
            ResetButtonColors();

            conn->Close();

            if (dt->Rows->Count == 0) {
                MessageBox::Show("По вашему запросу ничего не найдено", "Результаты поиска",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка поиска: " + ex->Message, "Ошибка",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
    };
}