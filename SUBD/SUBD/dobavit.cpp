#include "dobavit.h"
#include "AdminPanel.h"

namespace SUBD {

    dobavit::dobavit(AdminPanel^ parentAdminPanel)
    {
        adminPanel = parentAdminPanel;
        connStr = "DSN=AliansDB;UID=postgres;PWD=Azard18031;";
        playersList = gcnew System::Collections::Generic::Dictionary<int, String^>();

        // Инициализация цветов
        primaryColor = Color::FromArgb(41, 128, 185);
        secondaryColor = Color::FromArgb(52, 152, 219);
        accentColor = Color::FromArgb(231, 76, 60);
        backgroundColor = Color::FromArgb(245, 245, 245);
        textColor = Color::FromArgb(52, 73, 94);
        borderColor = Color::FromArgb(189, 195, 199);

        InitializeComponent();
        LoadPlayersList();
    }

    dobavit::~dobavit()
    {
        if (components) delete components;
    }

    void dobavit::ApplyModernStyle(Control^ control) {
        if (control->GetType() == TextBox::typeid) {
            TextBox^ textBox = safe_cast<TextBox^>(control);
            textBox->BackColor = Color::White;
            textBox->ForeColor = textColor;
            textBox->BorderStyle = BorderStyle::FixedSingle;
            textBox->Font = gcnew Drawing::Font("Segoe UI", 10);
        }
        else if (control->GetType() == ComboBox::typeid) {
            ComboBox^ combo = safe_cast<ComboBox^>(control);
            combo->BackColor = Color::White;
            combo->ForeColor = textColor;
            combo->FlatStyle = FlatStyle::Flat;
            combo->Font = gcnew Drawing::Font("Segoe UI", 10);
        }
        else if (control->GetType() == Label::typeid) {
            Label^ label = safe_cast<Label^>(control);
            label->ForeColor = textColor;
            label->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
        }
        else if (control->GetType() == DateTimePicker::typeid) {
            DateTimePicker^ dtp = safe_cast<DateTimePicker^>(control);
            dtp->BackColor = Color::White;
            dtp->ForeColor = textColor;
            dtp->Font = gcnew Drawing::Font("Segoe UI", 10);
        }
        else if (control->GetType() == Panel::typeid) {
            Panel^ panel = safe_cast<Panel^>(control);
            panel->BackColor = Color::White;
            panel->BorderStyle = BorderStyle::FixedSingle;
        }
        else if (control->GetType() == CheckBox::typeid) {
            CheckBox^ checkBox = safe_cast<CheckBox^>(control);
            checkBox->ForeColor = textColor;
            checkBox->Font = gcnew Drawing::Font("Segoe UI", 9);
        }
    }

    void dobavit::ApplyButtonStyle(Button^ button, bool isPrimary) {
        button->FlatStyle = FlatStyle::Flat;
        button->FlatAppearance->BorderSize = 0;
        button->ForeColor = Color::White;
        button->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
        button->Cursor = Cursors::Hand;

        if (isPrimary) {
            button->BackColor = primaryColor;
            button->FlatAppearance->MouseOverBackColor = secondaryColor;
            button->FlatAppearance->MouseDownBackColor = Color::FromArgb(31, 97, 141);
        }
        else {
            button->BackColor = Color::FromArgb(149, 165, 166);
            button->FlatAppearance->MouseOverBackColor = Color::FromArgb(127, 140, 141);
            button->FlatAppearance->MouseDownBackColor = Color::FromArgb(108, 122, 125);
        }
    }

    void dobavit::InitializeComponent(void)
    {
        this->components = gcnew System::ComponentModel::Container();
        // УВЕЛИЧИВАЕМ ФОРМУ
        this->Size = System::Drawing::Size(550, 800);
        this->Text = L"Добавить данные";
        this->StartPosition = FormStartPosition::CenterScreen;
        this->BackColor = backgroundColor;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;

        // Header Panel - УВЕЛИЧИВАЕМ ШИРИНУ
        this->headerPanel = gcnew Panel();
        this->headerPanel->Location = Point(0, 0);
        this->headerPanel->Size = Drawing::Size(550, 60);
        this->headerPanel->BackColor = primaryColor;

        this->titleLabel = gcnew Label();
        this->titleLabel->Text = L"ДОБАВЛЕНИЕ ДАННЫХ";
        this->titleLabel->Location = Point(20, 15);
        this->titleLabel->Size = Drawing::Size(510, 30);
        this->titleLabel->ForeColor = Color::White;
        this->titleLabel->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Bold);
        this->titleLabel->TextAlign = ContentAlignment::MiddleLeft;

        this->headerPanel->Controls->Add(this->titleLabel);

        // Content Panel - УВЕЛИЧИВАЕМ
        this->contentPanel = gcnew Panel();
        this->contentPanel->Location = Point(20, 80);
        this->contentPanel->Size = Drawing::Size(500, 600);  // Уменьшаем высоту для кнопок
        this->contentPanel->BackColor = Color::White;

        // Footer Panel - ПЕРЕМЕЩАЕМ НИЖЕ
        this->footerPanel = gcnew Panel();
        this->footerPanel->Location = Point(20, 700);  // Под contentPanel
        this->footerPanel->Size = Drawing::Size(500, 80);
        this->footerPanel->BackColor = Color::FromArgb(250, 250, 250);


        // ComboBox для выбора типа данных - УВЕЛИЧИВАЕМ ШИРИНУ
        this->comboBoxType = gcnew ComboBox();
        this->comboBoxType->Location = Point(15, 15);
        this->comboBoxType->Size = Drawing::Size(480, 25);
        this->comboBoxType->DropDownStyle = ComboBoxStyle::DropDownList;
        this->comboBoxType->Items->AddRange(gcnew array<String^> {
            L"Игрок", L"Матч"
        });
        this->comboBoxType->SelectedIndexChanged += gcnew EventHandler(this, &dobavit::comboBoxType_SelectedIndexChanged);

        this->labelType = gcnew Label();
        this->labelType->Location = Point(15, -5);
        this->labelType->Size = Drawing::Size(200, 20);
        this->labelType->Text = L"Тип данных:";

        // Поля для игрока - УВЕЛИЧИВАЕМ ШИРИНУ
        this->labelName = gcnew Label();
        this->labelName->Location = Point(15, 65);
        this->labelName->Size = Drawing::Size(200, 20);
        this->labelName->Text = L"Имя игрока:";
        this->labelName->Visible = false;

        this->textBoxName = gcnew TextBox();
        this->textBoxName->Location = Point(15, 85);
        this->textBoxName->Size = Drawing::Size(480, 25);
        this->textBoxName->Visible = false;

        this->labelPosition = gcnew Label();
        this->labelPosition->Location = Point(15, 130);
        this->labelPosition->Size = Drawing::Size(200, 20);
        this->labelPosition->Text = L"Позиция:";
        this->labelPosition->Visible = false;

        this->comboBoxPosition = gcnew ComboBox();
        this->comboBoxPosition->Location = Point(15, 150);
        this->comboBoxPosition->Size = Drawing::Size(480, 25);
        this->comboBoxPosition->DropDownStyle = ComboBoxStyle::DropDownList;
        this->comboBoxPosition->Items->AddRange(gcnew array<String^> {
            L"Вратарь", L"Нападающий", L"Центральный защитник",
                L"Левый защитник", L"Правый защитник", L"Полузащитник"
        });
        this->comboBoxPosition->Visible = false;
        this->comboBoxPosition->SelectedIndexChanged += gcnew EventHandler(this, &dobavit::comboBoxPosition_SelectedIndexChanged);

        // Статистика для полевых игроков - УВЕЛИЧИВАЕМ ШИРИНУ
        this->labelMatches = gcnew Label();
        this->labelMatches->Location = Point(15, 195);
        this->labelMatches->Size = Drawing::Size(200, 20);
        this->labelMatches->Text = L"Матчи:";
        this->labelMatches->Visible = false;

        this->textBoxMatches = gcnew TextBox();
        this->textBoxMatches->Location = Point(15, 215);
        this->textBoxMatches->Size = Drawing::Size(480, 25);
        this->textBoxMatches->Visible = false;

        this->labelGoals = gcnew Label();
        this->labelGoals->Location = Point(15, 260);
        this->labelGoals->Size = Drawing::Size(200, 20);
        this->labelGoals->Text = L"Голы:";
        this->labelGoals->Visible = false;

        this->textBoxGoals = gcnew TextBox();
        this->textBoxGoals->Location = Point(15, 280);
        this->textBoxGoals->Size = Drawing::Size(480, 25);
        this->textBoxGoals->Visible = false;

        this->labelAssists = gcnew Label();
        this->labelAssists->Location = Point(15, 325);
        this->labelAssists->Size = Drawing::Size(200, 20);
        this->labelAssists->Text = L"Голевые передачи:";
        this->labelAssists->Visible = false;

        this->textBoxAssists = gcnew TextBox();
        this->textBoxAssists->Location = Point(15, 345);
        this->textBoxAssists->Size = Drawing::Size(480, 25);
        this->textBoxAssists->Visible = false;

        // Статистика для вратарей - УВЕЛИЧИВАЕМ ШИРИНУ
        this->labelConceded = gcnew Label();
        this->labelConceded->Location = Point(15, 260);
        this->labelConceded->Size = Drawing::Size(200, 20);
        this->labelConceded->Text = L"Пропущенные мячи:";
        this->labelConceded->Visible = false;

        this->textBoxConceded = gcnew TextBox();
        this->textBoxConceded->Location = Point(15, 280);
        this->textBoxConceded->Size = Drawing::Size(480, 25);
        this->textBoxConceded->Visible = false;

        // Поля для матча - УВЕЛИЧИВАЕМ ШИРИНУ
        this->labelHomeTeam = gcnew Label();
        this->labelHomeTeam->Location = Point(15, 65);
        this->labelHomeTeam->Size = Drawing::Size(200, 20);
        this->labelHomeTeam->Text = L"Домашняя команда:";
        this->labelHomeTeam->Visible = false;

        this->textBoxHomeTeam = gcnew TextBox();
        this->textBoxHomeTeam->Location = Point(15, 85);
        this->textBoxHomeTeam->Size = Drawing::Size(480, 25);
        this->textBoxHomeTeam->Visible = false;

        this->labelGuestTeam = gcnew Label();
        this->labelGuestTeam->Location = Point(15, 130);
        this->labelGuestTeam->Size = Drawing::Size(200, 20);
        this->labelGuestTeam->Text = L"Команда гостей:";
        this->labelGuestTeam->Visible = false;

        this->textBoxGuestTeam = gcnew TextBox();
        this->textBoxGuestTeam->Location = Point(15, 150);
        this->textBoxGuestTeam->Size = Drawing::Size(480, 25);
        this->textBoxGuestTeam->Visible = false;

        this->labelHomeGoals = gcnew Label();
        this->labelHomeGoals->Location = Point(15, 195);
        this->labelHomeGoals->Size = Drawing::Size(200, 20);
        this->labelHomeGoals->Text = L"Голы домашней команды:";
        this->labelHomeGoals->Visible = false;

        this->textBoxHomeGoals = gcnew TextBox();
        this->textBoxHomeGoals->Location = Point(15, 215);
        this->textBoxHomeGoals->Size = Drawing::Size(480, 25);
        this->textBoxHomeGoals->Visible = false;

        this->labelGuestGoals = gcnew Label();
        this->labelGuestGoals->Location = Point(15, 260);
        this->labelGuestGoals->Size = Drawing::Size(200, 20);
        this->labelGuestGoals->Text = L"Голы гостей:";
        this->labelGuestGoals->Visible = false;

        this->textBoxGuestGoals = gcnew TextBox();
        this->textBoxGuestGoals->Location = Point(15, 280);
        this->textBoxGuestGoals->Size = Drawing::Size(480, 25);
        this->textBoxGuestGoals->Visible = false;

        this->labelDate = gcnew Label();
        this->labelDate->Location = Point(15, 325);
        this->labelDate->Size = Drawing::Size(200, 20);
        this->labelDate->Text = L"Дата матча:";
        this->labelDate->Visible = false;

        this->dateTimePicker = gcnew DateTimePicker();
        this->dateTimePicker->Location = Point(15, 345);
        this->dateTimePicker->Size = Drawing::Size(480, 25);
        this->dateTimePicker->Visible = false;

        // НОВЫЕ ПОЛЯ: Авторы голов с количеством - УВЕЛИЧИВАЕМ
        this->labelGoalScorers = gcnew Label();
        this->labelGoalScorers->Location = Point(15, 390);
        this->labelGoalScorers->Size = Drawing::Size(480, 20);
        this->labelGoalScorers->Text = L"Авторы голов (выберите игроков и укажите количество):";
        this->labelGoalScorers->Visible = false;

        this->panelGoalScorers = gcnew Panel();
        this->panelGoalScorers->Location = Point(15, 410);
        this->panelGoalScorers->Size = Drawing::Size(480, 100);
        this->panelGoalScorers->AutoScroll = true;
        this->panelGoalScorers->Visible = false;
        this->panelGoalScorers->BorderStyle = BorderStyle::FixedSingle;

        // НОВЫЕ ПОЛЯ: Авторы голевых передач с количеством - УВЕЛИЧИВАЕМ И ПЕРЕМЕЩАЕМ НИЖЕ
        this->labelAssistProviders = gcnew Label();
        this->labelAssistProviders->Location = Point(15, 520);
        this->labelAssistProviders->Size = Drawing::Size(480, 20);
        this->labelAssistProviders->Text = L"Авторы голевых передач (выберите игроков и укажите количество):";
        this->labelAssistProviders->Visible = false;

        this->panelAssistProviders = gcnew Panel();
        this->panelAssistProviders->Location = Point(15, 540);
        this->panelAssistProviders->Size = Drawing::Size(480, 100);
        this->panelAssistProviders->AutoScroll = true;
        this->panelAssistProviders->Visible = false;
        this->panelAssistProviders->BorderStyle = BorderStyle::FixedSingle;

        // Кнопки - ИСПРАВЛЯЕМ РАЗМЕР И РАСПОЛОЖЕНИЕ
        this->btnAdd = gcnew Button();
        this->btnAdd->Location = Point(50, 20);
        this->btnAdd->Size = Drawing::Size(180, 40);
        this->btnAdd->Text = L"ДОБАВИТЬ";
        this->btnAdd->Click += gcnew EventHandler(this, &dobavit::btnAdd_Click);

        this->btnCancel = gcnew Button();
        this->btnCancel->Location = Point(270, 20);
        this->btnCancel->Size = Drawing::Size(180, 40);
        this->btnCancel->Text = L"ОТМЕНА";
        this->btnCancel->Click += gcnew EventHandler(this, &dobavit::btnCancel_Click);

        // Добавление контролов на панели
        this->contentPanel->Controls->Add(this->comboBoxType);
        this->contentPanel->Controls->Add(this->labelType);
        this->contentPanel->Controls->Add(this->labelName);
        this->contentPanel->Controls->Add(this->textBoxName);
        this->contentPanel->Controls->Add(this->labelPosition);
        this->contentPanel->Controls->Add(this->comboBoxPosition);
        this->contentPanel->Controls->Add(this->labelMatches);
        this->contentPanel->Controls->Add(this->textBoxMatches);
        this->contentPanel->Controls->Add(this->labelGoals);
        this->contentPanel->Controls->Add(this->textBoxGoals);
        this->contentPanel->Controls->Add(this->labelAssists);
        this->contentPanel->Controls->Add(this->textBoxAssists);
        this->contentPanel->Controls->Add(this->labelConceded);
        this->contentPanel->Controls->Add(this->textBoxConceded);
        this->contentPanel->Controls->Add(this->labelHomeTeam);
        this->contentPanel->Controls->Add(this->textBoxHomeTeam);
        this->contentPanel->Controls->Add(this->labelGuestTeam);
        this->contentPanel->Controls->Add(this->textBoxGuestTeam);
        this->contentPanel->Controls->Add(this->labelHomeGoals);
        this->contentPanel->Controls->Add(this->textBoxHomeGoals);
        this->contentPanel->Controls->Add(this->labelGuestGoals);
        this->contentPanel->Controls->Add(this->textBoxGuestGoals);
        this->contentPanel->Controls->Add(this->labelDate);
        this->contentPanel->Controls->Add(this->dateTimePicker);
        this->contentPanel->Controls->Add(this->labelGoalScorers);
        this->contentPanel->Controls->Add(this->panelGoalScorers);
        this->contentPanel->Controls->Add(this->labelAssistProviders);
        this->contentPanel->Controls->Add(this->panelAssistProviders);

        this->footerPanel->Controls->Add(this->btnAdd);
        this->footerPanel->Controls->Add(this->btnCancel);

        // Добавление панелей на форму
        this->Controls->Add(this->headerPanel);
        this->Controls->Add(this->contentPanel);
        this->Controls->Add(this->footerPanel);

       
        // Применение стилей ко всем контролам
        for each (Control ^ control in this->contentPanel->Controls) {
            ApplyModernStyle(control);
        }

        // Стили для кнопок
        ApplyButtonStyle(btnAdd, true);
        ApplyButtonStyle(btnCancel, false);
    }

    // Остальные методы остаются без изменений...
    System::Void dobavit::comboBoxType_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
        HideAllFields();

        if (comboBoxType->SelectedItem->ToString() == "Игрок") {
            ShowPlayerFields();
        }
        else if (comboBoxType->SelectedItem->ToString() == "Матч") {
            ShowMatchFields();
        }
    }

    System::Void dobavit::comboBoxPosition_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
        if (comboBoxPosition->SelectedItem->ToString() == "Вратарь") {
            labelGoals->Visible = false;
            textBoxGoals->Visible = false;
            labelAssists->Visible = false;
            textBoxAssists->Visible = false;
            labelConceded->Visible = true;
            textBoxConceded->Visible = true;
        }
        else {
            labelGoals->Visible = true;
            textBoxGoals->Visible = true;
            labelAssists->Visible = true;
            textBoxAssists->Visible = true;
            labelConceded->Visible = false;
            textBoxConceded->Visible = false;
        }
    }

    System::Void dobavit::btnAdd_Click(System::Object^ sender, System::EventArgs^ e) {
        if (comboBoxType->SelectedItem->ToString() == "Игрок") {
            AddPlayer();
        }
        else if (comboBoxType->SelectedItem->ToString() == "Матч") {
            AddMatch();
        }
    }

    System::Void dobavit::btnCancel_Click(System::Object^ sender, System::EventArgs^ e) {
        this->Close();
    }

    void dobavit::HideAllFields() {
        // Скрываем все поля игрока
        labelName->Visible = false;
        textBoxName->Visible = false;
        labelPosition->Visible = false;
        comboBoxPosition->Visible = false;
        labelMatches->Visible = false;
        textBoxMatches->Visible = false;
        labelGoals->Visible = false;
        textBoxGoals->Visible = false;
        labelAssists->Visible = false;
        textBoxAssists->Visible = false;
        labelConceded->Visible = false;
        textBoxConceded->Visible = false;

        // Скрываем все поля матча
        labelHomeTeam->Visible = false;
        textBoxHomeTeam->Visible = false;
        labelGuestTeam->Visible = false;
        textBoxGuestTeam->Visible = false;
        labelHomeGoals->Visible = false;
        textBoxHomeGoals->Visible = false;
        labelGuestGoals->Visible = false;
        textBoxGuestGoals->Visible = false;
        labelDate->Visible = false;
        dateTimePicker->Visible = false;

        // Скрываем новые поля
        labelGoalScorers->Visible = false;
        panelGoalScorers->Visible = false;
        labelAssistProviders->Visible = false;
        panelAssistProviders->Visible = false;
    }

    void dobavit::ShowPlayerFields() {
        labelName->Visible = true;
        textBoxName->Visible = true;
        labelPosition->Visible = true;
        comboBoxPosition->Visible = true;
        labelMatches->Visible = true;
        textBoxMatches->Visible = true;

        // Показываем соответствующие поля в зависимости от позиции
        if (comboBoxPosition->SelectedItem != nullptr &&
            comboBoxPosition->SelectedItem->ToString() == "Вратарь") {
            labelConceded->Visible = true;
            textBoxConceded->Visible = true;
        }
        else {
            labelGoals->Visible = true;
            textBoxGoals->Visible = true;
            labelAssists->Visible = true;
            textBoxAssists->Visible = true;
        }
    }

    void dobavit::ShowMatchFields() {
        labelHomeTeam->Visible = true;
        textBoxHomeTeam->Visible = true;
        labelGuestTeam->Visible = true;
        textBoxGuestTeam->Visible = true;
        labelHomeGoals->Visible = true;
        textBoxHomeGoals->Visible = true;
        labelGuestGoals->Visible = true;
        textBoxGuestGoals->Visible = true;
        labelDate->Visible = true;
        dateTimePicker->Visible = true;

        // Показываем новые поля
        labelGoalScorers->Visible = true;
        panelGoalScorers->Visible = true;
        labelAssistProviders->Visible = true;
        panelAssistProviders->Visible = true;

        // Заполняем панели игроками с чекбоксами и полями для ввода количества
        CreatePlayerSelectionPanels();
    }

    void dobavit::CreatePlayerSelectionPanels() {
        // Очищаем панели
        panelGoalScorers->Controls->Clear();
        panelAssistProviders->Controls->Clear();

        int yPos = 10;

        for each (KeyValuePair<int, String^> ^ player in playersList) {
            String^ playerInfo = player->Value + " (ID: " + player->Key + ")";

            // Создаем элементы для авторов голов
            CheckBox^ goalCheckBox = gcnew CheckBox();
            goalCheckBox->Text = playerInfo;
            goalCheckBox->Location = Point(10, yPos);
            goalCheckBox->Size = Drawing::Size(250, 20);
            goalCheckBox->Tag = player->Key;

            TextBox^ goalsCountTextBox = gcnew TextBox();
            goalsCountTextBox->Location = Point(270, yPos);
            goalsCountTextBox->Size = Drawing::Size(60, 20);
            goalsCountTextBox->Text = "0";
            goalsCountTextBox->Enabled = false;

            // Привязываем включение/выключение текстового поля к чекбоксу
            goalCheckBox->CheckedChanged += gcnew EventHandler(this, &dobavit::GoalCheckBox_CheckedChanged);

            panelGoalScorers->Controls->Add(goalCheckBox);
            panelGoalScorers->Controls->Add(goalsCountTextBox);

            // Создаем элементы для авторов голевых передач
            CheckBox^ assistCheckBox = gcnew CheckBox();
            assistCheckBox->Text = playerInfo;
            assistCheckBox->Location = Point(10, yPos);
            assistCheckBox->Size = Drawing::Size(250, 20);
            assistCheckBox->Tag = player->Key;

            TextBox^ assistsCountTextBox = gcnew TextBox();
            assistsCountTextBox->Location = Point(270, yPos);
            assistsCountTextBox->Size = Drawing::Size(60, 20);
            assistsCountTextBox->Text = "0";
            assistsCountTextBox->Enabled = false;

            // Привязываем включение/выключение текстового поля к чекбоксу
            assistCheckBox->CheckedChanged += gcnew EventHandler(this, &dobavit::AssistCheckBox_CheckedChanged);

            panelAssistProviders->Controls->Add(assistCheckBox);
            panelAssistProviders->Controls->Add(assistsCountTextBox);

            // Применяем стили
            ApplyModernStyle(goalCheckBox);
            ApplyModernStyle(goalsCountTextBox);
            ApplyModernStyle(assistCheckBox);
            ApplyModernStyle(assistsCountTextBox);

            yPos += 30;
        }
    }

    System::Void dobavit::GoalCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
        CheckBox^ checkBox = safe_cast<CheckBox^>(sender);
        // Находим соответствующее текстовое поле
        for each (Control ^ control in panelGoalScorers->Controls) {
            if (control->GetType() == TextBox::typeid && control->Location.X == 270 &&
                control->Location.Y == checkBox->Location.Y) {
                TextBox^ textBox = safe_cast<TextBox^>(control);
                textBox->Enabled = checkBox->Checked;
                if (!checkBox->Checked) {
                    textBox->Text = "0";
                }
                break;
            }
        }
    }

    System::Void dobavit::AssistCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
        CheckBox^ checkBox = safe_cast<CheckBox^>(sender);
        // Находим соответствующее текстовое поле
        for each (Control ^ control in panelAssistProviders->Controls) {
            if (control->GetType() == TextBox::typeid && control->Location.X == 270 &&
                control->Location.Y == checkBox->Location.Y) {
                TextBox^ textBox = safe_cast<TextBox^>(control);
                textBox->Enabled = checkBox->Checked;
                if (!checkBox->Checked) {
                    textBox->Text = "0";
                }
                break;
            }
        }
    }

    void dobavit::AddPlayer() {
        if (String::IsNullOrEmpty(textBoxName->Text) || comboBoxPosition->SelectedIndex == -1) {
            MessageBox::Show("Заполните все обязательные поля!");
            return;
        }

        try {
            OdbcConnection^ conn = gcnew OdbcConnection(connStr);
            conn->Open();

            int nextId = GetNextAvailableId("Player", conn);

            OdbcCommand^ cmdPlayer = gcnew OdbcCommand("INSERT INTO \"Player\" (id, \"Имя\", \"Позиция\") VALUES (?, ?, ?)", conn);
            cmdPlayer->Parameters->AddWithValue("id", nextId);
            cmdPlayer->Parameters->AddWithValue("name", textBoxName->Text);
            cmdPlayer->Parameters->AddWithValue("position", comboBoxPosition->SelectedItem->ToString());
            cmdPlayer->ExecuteNonQuery();

            String^ position = comboBoxPosition->SelectedItem->ToString();
            if (position == "Вратарь") {
                OdbcCommand^ cmdGoalkeeper = gcnew OdbcCommand("INSERT INTO \"Statgp\" (id, \"матчи\", \"пропущенные мячи\") VALUES (?, ?, ?)", conn);
                cmdGoalkeeper->Parameters->AddWithValue("id", nextId);
                cmdGoalkeeper->Parameters->AddWithValue("matches", String::IsNullOrEmpty(textBoxMatches->Text) ? 0 : Convert::ToInt32(textBoxMatches->Text));
                cmdGoalkeeper->Parameters->AddWithValue("conceded", String::IsNullOrEmpty(textBoxConceded->Text) ? 0 : Convert::ToInt32(textBoxConceded->Text));
                cmdGoalkeeper->ExecuteNonQuery();
            }
            else {
                OdbcCommand^ cmdFieldPlayer = gcnew OdbcCommand("INSERT INTO \"Statistika\" (id, \"матчи\", \"голы\", \"голевые передачи\") VALUES (?, ?, ?, ?)", conn);
                cmdFieldPlayer->Parameters->AddWithValue("id", nextId);
                cmdFieldPlayer->Parameters->AddWithValue("matches", String::IsNullOrEmpty(textBoxMatches->Text) ? 0 : Convert::ToInt32(textBoxMatches->Text));
                cmdFieldPlayer->Parameters->AddWithValue("goals", String::IsNullOrEmpty(textBoxGoals->Text) ? 0 : Convert::ToInt32(textBoxGoals->Text));
                cmdFieldPlayer->Parameters->AddWithValue("assists", String::IsNullOrEmpty(textBoxAssists->Text) ? 0 : Convert::ToInt32(textBoxAssists->Text));
                cmdFieldPlayer->ExecuteNonQuery();
            }

            conn->Close();

            MessageBox::Show("Игрок успешно добавлен!");

            adminPanel->LoadPlayers();
            adminPanel->LoadStats();
            LoadPlayersList(); // Обновляем список игроков

            this->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка при добавлении игрока: " + ex->Message);
        }
    }

    void dobavit::AddMatch() {
        if (String::IsNullOrEmpty(textBoxHomeTeam->Text) || String::IsNullOrEmpty(textBoxGuestTeam->Text)) {
            MessageBox::Show("Заполните названия команд!");
            return;
        }

        try {
            OdbcConnection^ conn = gcnew OdbcConnection(connStr);
            conn->Open();

            // Добавляем матч
            int nextId = GetNextAvailableId("igri", conn);

            OdbcCommand^ cmd = gcnew OdbcCommand("INSERT INTO \"igri\" (id, date, hause, gast, goalhause, goalgast) VALUES (?, ?, ?, ?, ?, ?)", conn);
            cmd->Parameters->AddWithValue("id", nextId);
            cmd->Parameters->AddWithValue("date", dateTimePicker->Value);
            cmd->Parameters->AddWithValue("hause", textBoxHomeTeam->Text);
            cmd->Parameters->AddWithValue("gast", textBoxGuestTeam->Text);
            cmd->Parameters->AddWithValue("goalhause", String::IsNullOrEmpty(textBoxHomeGoals->Text) ? 0 : Convert::ToInt32(textBoxHomeGoals->Text));
            cmd->Parameters->AddWithValue("goalgast", String::IsNullOrEmpty(textBoxGuestGoals->Text) ? 0 : Convert::ToInt32(textBoxGuestGoals->Text));
            cmd->ExecuteNonQuery();

            // НОВЫЙ ФУНКЦИОНАЛ: Обновляем счетчик матчей у всех игроков
            UpdateAllPlayersMatches(conn);

            // Обрабатываем авторов голов с количеством
            for each (Control ^ control in panelGoalScorers->Controls) {
                if (control->GetType() == CheckBox::typeid) {
                    CheckBox^ checkBox = safe_cast<CheckBox^>(control);
                    if (checkBox->Checked) {
                        int playerId = safe_cast<int>(checkBox->Tag);
                        // Находим соответствующее текстовое поле с количеством голов
                        for each (Control ^ txtControl in panelGoalScorers->Controls) {
                            if (txtControl->GetType() == TextBox::typeid &&
                                txtControl->Location.X == 270 &&
                                txtControl->Location.Y == checkBox->Location.Y) {
                                TextBox^ goalsTextBox = safe_cast<TextBox^>(txtControl);
                                int goalsCount = String::IsNullOrEmpty(goalsTextBox->Text) ? 0 : Convert::ToInt32(goalsTextBox->Text);
                                if (goalsCount > 0) {
                                    UpdatePlayerGoals(playerId, goalsCount, conn);
                                }
                                break;
                            }
                        }
                    }
                }
            }

            // Обрабатываем авторов голевых передач с количеством
            for each (Control ^ control in panelAssistProviders->Controls) {
                if (control->GetType() == CheckBox::typeid) {
                    CheckBox^ checkBox = safe_cast<CheckBox^>(control);
                    if (checkBox->Checked) {
                        int playerId = safe_cast<int>(checkBox->Tag);
                        // Находим соответствующее текстовое поле с количеством передач
                        for each (Control ^ txtControl in panelAssistProviders->Controls) {
                            if (txtControl->GetType() == TextBox::typeid &&
                                txtControl->Location.X == 270 &&
                                txtControl->Location.Y == checkBox->Location.Y) {
                                TextBox^ assistsTextBox = safe_cast<TextBox^>(txtControl);
                                int assistsCount = String::IsNullOrEmpty(assistsTextBox->Text) ? 0 : Convert::ToInt32(assistsTextBox->Text);
                                if (assistsCount > 0) {
                                    UpdatePlayerAssists(playerId, assistsCount, conn);
                                }
                                break;
                            }
                        }
                    }
                }
            }

            conn->Close();

            MessageBox::Show("Матч успешно добавлен! Статистика игроков обновлена.");

            adminPanel->LoadMatches();
            adminPanel->LoadStats();

            this->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка при добавлении матча: " + ex->Message);
        }
    }

    // НОВЫЙ МЕТОД: Обновляет счетчик матчей у всех игроков
    void dobavit::UpdateAllPlayersMatches(OdbcConnection^ conn) {
        try {
            // Обновляем матчи у полевых игроков
            OdbcCommand^ updateFieldPlayersCmd = gcnew OdbcCommand(
                "UPDATE \"Statistika\" SET \"матчи\" = COALESCE(\"матчи\", 0) + 1 "
                "WHERE id IN (SELECT id FROM \"Player\" WHERE \"Позиция\" != 'Вратарь')", conn);
            updateFieldPlayersCmd->ExecuteNonQuery();

            // Обновляем матчи у вратарей
            OdbcCommand^ updateGoalkeepersCmd = gcnew OdbcCommand(
                "UPDATE \"Statgp\" SET \"матчи\" = COALESCE(\"матчи\", 0) + 1 "
                "WHERE id IN (SELECT id FROM \"Player\" WHERE \"Позиция\" = 'Вратарь')", conn);
            updateGoalkeepersCmd->ExecuteNonQuery();

        }
        catch (Exception^ ex) {
            // Игнорируем ошибки для простоты
        }
    }

    int dobavit::GetNextAvailableId(const String^ tableName, OdbcConnection^ conn)
    {
        try {
            // Просто получаем максимальный ID и добавляем 1
            String^ maxSql;
            if (tableName == "Player") {
                maxSql = "SELECT COALESCE(MAX(id), 0) FROM \"Player\"";
            }
            else if (tableName == "igri") {
                maxSql = "SELECT COALESCE(MAX(id), 0) FROM \"igri\"";
            }
            else {
                return 1;
            }

            OdbcCommand^ maxCmd = gcnew OdbcCommand(maxSql, conn);
            int maxId = Convert::ToInt32(maxCmd->ExecuteScalar());
            return maxId + 1;
        }
        catch (Exception^ ex) {
            return 1;
        }
    }

    void dobavit::LoadPlayersList()
    {
        try {
            OdbcConnection^ conn = gcnew OdbcConnection(connStr);
            conn->Open();

            OdbcCommand^ cmd = gcnew OdbcCommand("SELECT id, \"Имя\" FROM \"Player\" ORDER BY \"Имя\"", conn);
            OdbcDataReader^ reader = cmd->ExecuteReader();

            playersList->Clear();

            while (reader->Read()) {
                int playerId = reader->GetInt32(0);
                String^ playerName = reader->GetString(1);
                playersList->Add(playerId, playerName);
            }

            reader->Close();
            conn->Close();
        }
        catch (Exception^ ex) {
            // Можно закомментировать если мешает
            // MessageBox::Show("Ошибка загрузки списка игроков: " + ex->Message);
        }
    }

    void dobavit::UpdatePlayerGoals(int playerId, int goalsCount, OdbcConnection^ conn) {
        if (playerId == -1 || goalsCount <= 0) return;

        try {
            OdbcCommand^ updateCmd = gcnew OdbcCommand(
                "UPDATE \"Statistika\" SET \"голы\" = COALESCE(\"голы\", 0) + ? WHERE id = ?", conn);
            updateCmd->Parameters->AddWithValue("goals", goalsCount);
            updateCmd->Parameters->AddWithValue("id", playerId);

            int rowsAffected = updateCmd->ExecuteNonQuery();

            // Если игрок не найден в таблице Statistika, добавляем его
            if (rowsAffected == 0) {
                OdbcCommand^ insertCmd = gcnew OdbcCommand(
                    "INSERT INTO \"Statistika\" (id, \"матчи\", \"голы\", \"голевые передачи\") VALUES (?, 1, ?, 0)", conn);
                insertCmd->Parameters->AddWithValue("id", playerId);
                insertCmd->Parameters->AddWithValue("goals", goalsCount);
                insertCmd->ExecuteNonQuery();
            }
        }
        catch (Exception^ ex) {
            // Игнорируем ошибки для простоты
        }
    }

    void dobavit::UpdatePlayerAssists(int playerId, int assistsCount, OdbcConnection^ conn) {
        if (playerId == -1 || assistsCount <= 0) return;

        try {
            OdbcCommand^ updateCmd = gcnew OdbcCommand(
                "UPDATE \"Statistika\" SET \"голевые передачи\" = COALESCE(\"голевые передачи\", 0) + ? WHERE id = ?", conn);
            updateCmd->Parameters->AddWithValue("assists", assistsCount);
            updateCmd->Parameters->AddWithValue("id", playerId);

            int rowsAffected = updateCmd->ExecuteNonQuery();

            // Если игрок не найден в таблице Statistika, добавляем его
            if (rowsAffected == 0) {
                OdbcCommand^ insertCmd = gcnew OdbcCommand(
                    "INSERT INTO \"Statistika\" (id, \"матчи\", \"голы\", \"голевые передачи\") VALUES (?, 1, 0, ?)", conn);
                insertCmd->Parameters->AddWithValue("id", playerId);
                insertCmd->Parameters->AddWithValue("assists", assistsCount);
                insertCmd->ExecuteNonQuery();
            }
        }
        catch (Exception^ ex) {
            // Игнорируем ошибки для простоты
        }
    }
}