#pragma once
#using <System.Data.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

namespace SUBD {
    ref class dobavit;  // Forward declaration
    ref class LineupBatchForm;  // Forward declaration для формы массового ввода состава
    ref class BatchForm;  // Forward declaration для универсальной формы массового ввода
    ref class TrainingParticipantsForm;  // Forward declaration для формы просмотра участников тренировки
    ref class MatchLineupForm;  // Forward declaration для формы просмотра состава матча
    ref class MatchEventsBatchForm;  // Forward declaration для формы массового ввода событий матчей
    ref class MatchEventsViewForm;  // Forward declaration для формы просмотра событий матча
}

#include "MatchEventsViewForm.h"
#include "MatchEventsBatchForm.h"
#include "TrainingParticipantsForm.h"
#include "MatchLineupForm.h"
#include "LineupBatchForm.h"
#include "dobavit.h"
#include <msclr/marshal.h>

namespace SUBD {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Data::Odbc;
    using namespace System::Drawing;
    using namespace System::Drawing::Drawing2D;
    using namespace System::Timers;

    public ref class AdminPanel : public System::Windows::Forms::Form
    {
    public:
        AdminPanel(void)
        {
            InitializeComponent();
            InitializeAnimations();
            InitializeDatePicker();
            ApplyModernDesign();
        }

    protected:
        ~AdminPanel()
        {
            if (components) delete components;
            if (fadeTimer) delete fadeTimer;
            if (glowTimer) delete glowTimer;
        }

    private:
        System::ComponentModel::Container^ components;
        Form^ MyForm;
        System::Windows::Forms::Timer^ fadeTimer;
        System::Windows::Forms::Timer^ glowTimer;
        double glowPhase = 0.0;

        // Controls
        DataGridView^ dataGridPlayers;
        DataGridView^ dataGridStats;
        DataGridView^ dataGridMatches;
        DataGridView^ dataGridMatchEvents;
        DataGridView^ dataGridLineups;
        DataGridView^ dataGridInjuries;
        DataGridView^ dataGridTrainings;

        // Основные кнопки
        Button^ btnQuickLoad;
        Button^ btnQuickSave;
        Button^ btnAddData;
        Button^ btnBack;

        // Кнопки удаления
        Button^ btnDeletePlayer;
        Button^ btnDeleteMatch;
        Button^ btnDeleteMatchEvent;
        Button^ btnDeleteLineup;
        Button^ btnDeleteInjury;
        Button^ btnDeleteTraining;
        Button^ btnBatchLineup;  // Кнопка для массового ввода состава
        Button^ btnBatchTraining;  // Новая кнопка для массового ввода участников тренировок
        Button^ btnBatchMatchEvents;  // Новая кнопка для массового ввода событий матчей

        // Комбобокс выбора таблицы
        ComboBox^ cmbTableSelector;

        // Главное меню
        MenuStrip^ mainMenu;
        ToolStripMenuItem^ fileMenu;
        ToolStripMenuItem^ viewMenu;
        ToolStripMenuItem^ dataMenu;
        ToolStripMenuItem^ toolsMenu;

        // Панели
        Panel^ headerPanel;
        Panel^ mainPanel;
        Panel^ footerPanel;
        Panel^ contentPanel;
        Panel^ statsPanel;
        Label^ lblTitle;
        PictureBox^ logoPicture;
        Label^ lblSubtitle;
        Label^ lblTotalPlayers;
        Label^ lblTotalMatches;
        Label^ lblActiveUsers;

        // Date Picker
        MonthCalendar^ datePicker;
        Panel^ datePickerPanel;
        Button^ datePickerOk;
        Button^ datePickerCancel;
        DataGridView^ currentDataGridWithDate;
        DataGridViewCell^ currentDateCell;


        String^ connStr = "DSN=AliansDB;UID=postgres;PWD=Azard18031;";
        OdbcDataAdapter^ daPlayers;
        OdbcDataAdapter^ daStats;
        OdbcDataAdapter^ matchesAdapter;
        OdbcDataAdapter^ matchEventsAdapter;
        OdbcDataAdapter^ lineupsAdapter;
        OdbcDataAdapter^ injuriesAdapter;
        OdbcDataAdapter^ trainingsAdapter;

        DataTable^ dtPlayers;
        DataTable^ dtStats;
        DataTable^ matchesTable;
        DataTable^ matchEventsTable;
        DataTable^ lineupsTable;
        DataTable^ injuriesTable;
        DataTable^ trainingsTable;

        // Кэш данных для выпадающих списков
        DataTable^ playersCache;
        DataTable^ matchesCache;


        Color primaryColor = Color::FromArgb(41, 128, 185);
        Color secondaryColor = Color::FromArgb(52, 152, 219);
        Color accentColor = Color::FromArgb(231, 76, 60);
        Color successColor = Color::FromArgb(46, 204, 113);
        Color warningColor = Color::FromArgb(241, 196, 15);
        Color infoColor = Color::FromArgb(155, 89, 182);
        Color darkColor = Color::FromArgb(44, 62, 80);
        Color darkerColor = Color::FromArgb(33, 47, 60);
        Color lightColor = Color::FromArgb(236, 240, 241);
        Color cardColor = Color::FromArgb(255, 255, 255);
        Color shadowColor = Color::FromArgb(0, 0, 0, 100);

        void InitializeAnimations()
        {

            fadeTimer = gcnew System::Windows::Forms::Timer();
            fadeTimer->Interval = 20;
            fadeTimer->Tick += gcnew EventHandler(this, &AdminPanel::FadeInTick);


            glowTimer = gcnew System::Windows::Forms::Timer();
            glowTimer->Interval = 50;
            glowTimer->Tick += gcnew EventHandler(this, &AdminPanel::GlowTick);
            glowTimer->Start();
        }

        void InitializeDatePicker()
        {
            // Панель для DatePicker
            datePickerPanel = gcnew Panel();
            datePickerPanel->Size = System::Drawing::Size(300, 280);
            datePickerPanel->BackColor = cardColor;
            datePickerPanel->Visible = false;
            datePickerPanel->BorderStyle = BorderStyle::FixedSingle;
            datePickerPanel->Paint += gcnew PaintEventHandler(this, &AdminPanel::DatePickerPanel_Paint);

            // Календарик
            datePicker = gcnew MonthCalendar();
            datePicker->Location = Point(10, 10);
            datePicker->MaxSelectionCount = 1;
            datePicker->Font = gcnew System::Drawing::Font("Segoe UI", 9);

            // Кнопка OK
            datePickerOk = gcnew Button();
            datePickerOk->Text = "OK";
            datePickerOk->Size = System::Drawing::Size(80, 30);
            datePickerOk->Location = Point(60, 220);
            datePickerOk->BackColor = successColor;
            datePickerOk->ForeColor = Color::White;
            datePickerOk->FlatStyle = FlatStyle::Flat;
            datePickerOk->Click += gcnew EventHandler(this, &AdminPanel::DatePickerOk_Click);

            // Кнопка Cancel
            datePickerCancel = gcnew Button();
            datePickerCancel->Text = "Отмена";
            datePickerCancel->Size = System::Drawing::Size(80, 30);
            datePickerCancel->Location = Point(160, 220);
            datePickerCancel->BackColor = accentColor;
            datePickerCancel->ForeColor = Color::White;
            datePickerCancel->FlatStyle = FlatStyle::Flat;
            datePickerCancel->Click += gcnew EventHandler(this, &AdminPanel::DatePickerCancel_Click);

            // Добавляем элементы на панель
            datePickerPanel->Controls->Add(datePicker);
            datePickerPanel->Controls->Add(datePickerOk);
            datePickerPanel->Controls->Add(datePickerCancel);

            // Добавляем панель на форму
            this->Controls->Add(datePickerPanel);
        }

        void DatePickerPanel_Paint(Object^ sender, PaintEventArgs^ e)
        {
            // Стилизация панели DatePicker
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            FillRoundedRectangle(g, gcnew SolidBrush(cardColor), datePickerPanel->ClientRectangle, 8);
            DrawRoundedRectangle(g, gcnew Pen(Color::FromArgb(200, 200, 200)), datePickerPanel->ClientRectangle, 8);
        }

        void DatePickerOk_Click(Object^ sender, EventArgs^ e)
        {
            if (currentDateCell != nullptr && currentDataGridWithDate != nullptr) {
                currentDateCell->Value = datePicker->SelectionStart;
                currentDataGridWithDate->NotifyCurrentCellDirty(true);
                currentDataGridWithDate->EndEdit();
            }
            datePickerPanel->Visible = false;
        }

        void DatePickerCancel_Click(Object^ sender, EventArgs^ e)
        {
            datePickerPanel->Visible = false;
        }

        void ShowDatePicker(DataGridView^ dgv, DataGridViewCell^ cell)
        {
            currentDataGridWithDate = dgv;
            currentDateCell = cell;

            // Позиционируем панель рядом с ячейкой
            Point cellPosition = dgv->GetCellDisplayRectangle(cell->ColumnIndex, cell->RowIndex, false).Location;
            Point dgvPosition = dgv->PointToScreen(cellPosition);
            Point formPosition = this->PointToClient(dgvPosition);

            datePickerPanel->Location = Point(formPosition.X, formPosition.Y + 25);
            datePickerPanel->BringToFront();
            datePickerPanel->Visible = true;

            // Устанавливаем текущую дату из ячейки
            if (cell->Value != nullptr && cell->Value->GetType() == DateTime::typeid) {
                datePicker->SetDate(safe_cast<DateTime>(cell->Value));
            }
            else {
                datePicker->SetDate(DateTime::Now);
            }
        }

        void FadeInTick(Object^ sender, EventArgs^ e)
        {
            if (this->Opacity >= 1.0) {
                fadeTimer->Stop();
                return;
            }
            this->Opacity += 0.05;
        }

        void GlowTick(Object^ sender, EventArgs^ e)
        {
            glowPhase += 0.1;
            if (glowPhase > 2 * Math::PI) glowPhase = 0;
            UpdateButtonGlow();
        }

        void UpdateButtonGlow()
        {
            // Subtle glow effect for active buttons
            float glowIntensity = (float)(Math::Sin(glowPhase) * 0.1 + 0.9);

            array<Button^>^ mainButtons = {
                btnQuickLoad, btnQuickSave, btnAddData
            };

            for each (Button ^ btn in mainButtons) {
                if (btn->BackColor == primaryColor) {
                    btn->Invalidate();
                }
            }
        }

        void ApplyModernDesign()
        {
            // Form styling with modern look
            this->BackColor = Color::FromArgb(245, 247, 250);
            this->Font = gcnew System::Drawing::Font("Segoe UI", 9);
            this->Opacity = 0;
            fadeTimer->Start();

            // Header panel with gradient
            headerPanel->BackColor = darkColor;
            headerPanel->Dock = DockStyle::Top;
            headerPanel->Height = 70;
            headerPanel->Paint += gcnew PaintEventHandler(this, &AdminPanel::HeaderPanel_Paint);

            // Title label with modern font
            lblTitle->ForeColor = Color::White;
            lblTitle->Font = gcnew System::Drawing::Font("Segoe UI", 18, FontStyle::Bold);
            lblTitle->Text = "АДМИН-ПАНЕЛЬ МФК ВятГУ";
            lblTitle->AutoSize = true;
            lblTitle->Location = Point(80, 20);

            // Subtitle
            lblSubtitle->ForeColor = Color::FromArgb(200, 200, 200);
            lblSubtitle->Font = gcnew System::Drawing::Font("Segoe UI", 9);
            lblSubtitle->Text = "Управление базой данных футбольного клуба";
            lblSubtitle->AutoSize = true;
            lblSubtitle->Location = Point(80, 48);

            // Logo with modern style
            logoPicture->Size = System::Drawing::Size(50, 50);
            logoPicture->Location = Point(20, 10);
            logoPicture->BackColor = Color::Transparent;
            logoPicture->Paint += gcnew PaintEventHandler(this, &AdminPanel::Logo_Paint);

            // Main panel
            mainPanel->BackColor = Color::Transparent;
            mainPanel->Dock = DockStyle::Fill;

            // Content panel for better organization
            contentPanel->BackColor = Color::Transparent;
            contentPanel->Dock = DockStyle::Fill;

            // Stats panel
            InitializeStatsPanel();

            // Footer panel
            footerPanel->BackColor = darkColor;
            footerPanel->Dock = DockStyle::Bottom;
            footerPanel->Height = 35;

            // Apply modern button styles
            ApplyModernButtonStyle(btnQuickLoad, "", primaryColor);
            ApplyModernButtonStyle(btnQuickSave, "", successColor);
            ApplyModernButtonStyle(btnAddData, "", successColor);
            ApplyModernButtonStyle(btnBack, "", Color::FromArgb(108, 117, 125));

            // Кнопки удаления
            ApplyModernButtonStyle(btnDeletePlayer, "", accentColor);
            ApplyModernButtonStyle(btnDeleteMatch, "", accentColor);
            ApplyModernButtonStyle(btnDeleteMatchEvent, "", accentColor);
            ApplyModernButtonStyle(btnDeleteLineup, "", accentColor);
            ApplyModernButtonStyle(btnDeleteInjury, "", accentColor);
            ApplyModernButtonStyle(btnDeleteTraining, "", accentColor);
            ApplyModernButtonStyle(btnBatchLineup, "", infoColor);  // Стиль для кнопки составов
            ApplyModernButtonStyle(btnBatchTraining, "", Color::FromArgb(155, 89, 182));  // Стиль для кнопки тренировок
            ApplyModernButtonStyle(btnBatchMatchEvents, "", warningColor);  // Стиль для кнопки событий

            // Modern DataGridView styling
            StyleModernDataGridView(dataGridPlayers);
            StyleModernDataGridView(dataGridStats);
            StyleModernDataGridView(dataGridMatches);
            StyleModernDataGridView(dataGridMatchEvents);
            StyleModernDataGridView(dataGridLineups);
            StyleModernDataGridView(dataGridInjuries);
            StyleModernDataGridView(dataGridTrainings);

            // Настройка комбобокса
            cmbTableSelector->Font = gcnew System::Drawing::Font("Segoe UI", 9);
            cmbTableSelector->BackColor = cardColor;
            cmbTableSelector->ForeColor = darkColor;
        }

        void HeaderPanel_Paint(Object^ sender, PaintEventArgs^ e)
        {
            // Gradient background for header
            System::Drawing::Rectangle rect = headerPanel->ClientRectangle;
            LinearGradientBrush^ brush = gcnew LinearGradientBrush(
                rect, darkColor, darkerColor, LinearGradientMode::Vertical);
            e->Graphics->FillRectangle(brush, rect);
            delete brush;
        }

        void Logo_Paint(Object^ sender, PaintEventArgs^ e)
        {
            // Draw modern logo
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;

            // Football circle
            RectangleF circleRect = RectangleF(5, 5, 40, 40);
            g->FillEllipse(gcnew SolidBrush(primaryColor), circleRect);

            // Football pattern
            Pen^ whitePen = gcnew Pen(Color::White, 2);
            g->DrawLine(whitePen, 25, 10, 25, 40);
            g->DrawLine(whitePen, 10, 25, 40, 25);
            g->DrawArc(whitePen, 15, 15, 20, 20, 45, 180);
            delete whitePen;
        }

        void InitializeStatsPanel()
        {
            statsPanel->BackColor = cardColor;
            statsPanel->Size = System::Drawing::Size(1100, 80);
            statsPanel->Location = Point(20, 10);
            statsPanel->Paint += gcnew PaintEventHandler(this, &AdminPanel::StatsPanel_Paint);

            // Total players stat
            lblTotalPlayers->Font = gcnew System::Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            lblTotalPlayers->ForeColor = darkColor;
            lblTotalPlayers->Text = "Игроки: 0";
            lblTotalPlayers->AutoSize = true;
            lblTotalPlayers->Location = Point(30, 25);

            // Total matches stat
            lblTotalMatches->Font = gcnew System::Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            lblTotalMatches->ForeColor = darkColor;
            lblTotalMatches->Text = "Матчи: 0";
            lblTotalMatches->AutoSize = true;
            lblTotalMatches->Location = Point(200, 25);

            // Active users stat
            lblActiveUsers->Font = gcnew System::Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            lblActiveUsers->ForeColor = darkColor;
            lblActiveUsers->Text = "Статистика: Обновлено";
            lblActiveUsers->AutoSize = true;
            lblActiveUsers->Location = Point(370, 25);

            statsPanel->Controls->Add(lblTotalPlayers);
            statsPanel->Controls->Add(lblTotalMatches);
            statsPanel->Controls->Add(lblActiveUsers);
        }

        void StatsPanel_Paint(Object^ sender, PaintEventArgs^ e)
        {

            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;


            System::Drawing::Rectangle shadowRect = statsPanel->ClientRectangle;
            shadowRect.Offset(2, 2);
            FillRoundedRectangle(g, gcnew SolidBrush(shadowColor), shadowRect, 10);


            FillRoundedRectangle(g, gcnew SolidBrush(cardColor), statsPanel->ClientRectangle, 10);


            DrawRoundedRectangle(g, gcnew Pen(Color::FromArgb(240, 240, 240), 1), statsPanel->ClientRectangle, 10);
        }

        void ApplyModernButtonStyle(Button^ button, String^ icon, Color color)
        {
            // Сброс плоского стиля для восстановления теней и скруглений
            button->FlatStyle = FlatStyle::Standard;

            // Настройка отступов для разделения кнопок
            button->Margin = System::Windows::Forms::Padding(5, 5, 5, 5);

            button->BackColor = color;
            button->ForeColor = Color::White;
            button->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            button->Text = icon + "  " + button->Text;
            button->Size = System::Drawing::Size(140, 40);
            button->Cursor = Cursors::Hand;
            button->TextAlign = ContentAlignment::MiddleCenter;

            // Вместо этого используем стандартные границы
            button->FlatStyle = FlatStyle::Flat;
            button->FlatAppearance->BorderSize = 1;
            button->FlatAppearance->BorderColor = Color::FromArgb(180, 180, 180);

            // Обработчики событий для hover эффектов
            button->MouseEnter += gcnew EventHandler(this, &AdminPanel::ModernButton_MouseEnter);
            button->MouseLeave += gcnew EventHandler(this, &AdminPanel::ModernButton_MouseLeave);
            button->MouseDown += gcnew MouseEventHandler(this, &AdminPanel::ModernButton_MouseDown);
            button->MouseUp += gcnew MouseEventHandler(this, &AdminPanel::ModernButton_MouseUp);
        }

        void ModernButton_Paint(Object^ sender, PaintEventArgs^ e)
        {
            Button^ button = safe_cast<Button^>(sender);
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;


            System::Drawing::Rectangle shadowRect = button->ClientRectangle;
            shadowRect.Offset(0, 3);
            FillRoundedRectangle(g, gcnew SolidBrush(Color::FromArgb(30, 0, 0, 0)), shadowRect, 12);


            FillRoundedRectangle(g, gcnew SolidBrush(button->BackColor), button->ClientRectangle, 10);


            StringFormat^ sf = gcnew StringFormat();
            sf->Alignment = StringAlignment::Center;
            sf->LineAlignment = StringAlignment::Center;

            g->DrawString(button->Text, button->Font, gcnew SolidBrush(button->ForeColor), button->ClientRectangle, sf);

            delete sf;
        }

        void ModernButton_MouseEnter(Object^ sender, EventArgs^ e)
        {
            Button^ button = safe_cast<Button^>(sender);
            button->BackColor = ControlPaint::Light(button->BackColor, 0.15f);
            button->Invalidate();
        }

        void ModernButton_MouseLeave(Object^ sender, EventArgs^ e)
        {
            Button^ button = safe_cast<Button^>(sender);
            if (button == btnBack)
                button->BackColor = Color::FromArgb(108, 117, 125);
            else if (button == btnQuickLoad)
                button->BackColor = primaryColor;
            else if (button == btnQuickSave || button == btnAddData)
                button->BackColor = successColor;
            else if (button == btnDeletePlayer || button == btnDeleteMatch || button == btnDeleteMatchEvent ||
                button == btnDeleteLineup || button == btnDeleteInjury || button == btnDeleteTraining)
                button->BackColor = accentColor;
            else if (button == btnBatchLineup)
                button->BackColor = infoColor;
            else if (button == btnBatchTraining)
                button->BackColor = Color::FromArgb(155, 89, 182);  // Фиолетовый для тренировок
            else if (button == btnBatchMatchEvents)
                button->BackColor = warningColor;  // Желтый для событий
            button->Invalidate();
        }

        void ModernButton_MouseDown(Object^ sender, MouseEventArgs^ e)
        {
            Button^ button = safe_cast<Button^>(sender);
            button->BackColor = ControlPaint::Dark(button->BackColor, 0.2f);
            button->Invalidate();
        }

        void ModernButton_MouseUp(Object^ sender, MouseEventArgs^ e)
        {
            ModernButton_MouseEnter(sender, e);
        }

        void StyleModernDataGridView(DataGridView^ dgv)
        {
            dgv->BackgroundColor = cardColor;
            dgv->BorderStyle = BorderStyle::None;
            dgv->EnableHeadersVisualStyles = false;

            // Modern header style
            dgv->ColumnHeadersDefaultCellStyle->BackColor = primaryColor;
            dgv->ColumnHeadersDefaultCellStyle->ForeColor = Color::White;
            dgv->ColumnHeadersDefaultCellStyle->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            dgv->ColumnHeadersHeight = 40;
            dgv->ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle::None;

            // Modern row style
            dgv->DefaultCellStyle->Font = gcnew System::Drawing::Font("Segoe UI", 9);
            dgv->DefaultCellStyle->BackColor = cardColor;
            dgv->DefaultCellStyle->ForeColor = Color::FromArgb(60, 60, 60);
            dgv->DefaultCellStyle->SelectionBackColor = Color::FromArgb(229, 243, 255);
            dgv->DefaultCellStyle->SelectionForeColor = Color::Black;
            dgv->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(248, 250, 252);

            // Grid style
            dgv->GridColor = Color::FromArgb(230, 230, 230);
            dgv->RowHeadersVisible = false;
            dgv->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgv->RowTemplate->Height = 35;

            // Selection
            dgv->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgv->MultiSelect = false;

            // Modern border
            dgv->Paint += gcnew PaintEventHandler(this, &AdminPanel::DataGridView_Paint);
        }

        void DataGridView_Paint(Object^ sender, PaintEventArgs^ e)
        {
            DataGridView^ dgv = safe_cast<DataGridView^>(sender);
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;

            // Shadow effect
            System::Drawing::Rectangle shadowRect = dgv->ClientRectangle;
            shadowRect.Offset(2, 2);
            FillRoundedRectangle(g, gcnew SolidBrush(shadowColor), shadowRect, 5);

            // Border
            DrawRoundedRectangle(g, gcnew Pen(Color::FromArgb(220, 220, 220), 1), dgv->ClientRectangle, 5);
        }
        //счетчики
        void UpdateStats()
        {
            try {
                // Счетчик кол-ва игроков
                if (dtPlayers != nullptr) {
                    lblTotalPlayers->Text = String::Format(" Игроки: {0}", dtPlayers->Rows->Count);
                }

                // Счетчик Кол-ва матчей
                if (matchesTable != nullptr) {
                    lblTotalMatches->Text = String::Format(" Матчи: {0}", matchesTable->Rows->Count);
                }

                lblActiveUsers->Text = String::Format("Обновлено: {0:HH:mm:ss}", DateTime::Now);
            }
            catch (...) {}
        }


        void FillRoundedRectangle(Graphics^ g, Brush^ brush, System::Drawing::Rectangle rect, int radius)
        {
            GraphicsPath^ path = gcnew GraphicsPath();
            path->AddArc(rect.X, rect.Y, radius, radius, 180, 90);
            path->AddArc(rect.X + rect.Width - radius, rect.Y, radius, radius, 270, 90);
            path->AddArc(rect.X + rect.Width - radius, rect.Y + rect.Height - radius, radius, radius, 0, 90);
            path->AddArc(rect.X, rect.Y + rect.Height - radius, radius, radius, 90, 90);
            path->CloseFigure();
            g->FillPath(brush, path);
            delete path;
        }

        void DrawRoundedRectangle(Graphics^ g, Pen^ pen, System::Drawing::Rectangle rect, int radius)
        {
            GraphicsPath^ path = gcnew GraphicsPath();
            path->AddArc(rect.X, rect.Y, radius, radius, 180, 90);
            path->AddArc(rect.X + rect.Width - radius, rect.Y, radius, radius, 270, 90);
            path->AddArc(rect.X + rect.Width - radius, rect.Y + rect.Height - radius, radius, radius, 0, 90);
            path->AddArc(rect.X, rect.Y + rect.Height - radius, radius, radius, 90, 90);
            path->CloseFigure();
            g->DrawPath(pen, path);
            delete path;
        }

    public:
        void LoadPlayers()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                daPlayers = gcnew OdbcDataAdapter("SELECT id, \"Имя\", \"Позиция\" FROM \"Player\" ORDER BY id;", conn);
                dtPlayers = gcnew DataTable();
                daPlayers->Fill(dtPlayers);

                dataGridPlayers->DataSource = nullptr;
                dataGridPlayers->Columns->Clear();
                dataGridPlayers->DataSource = dtPlayers;

                if (dataGridPlayers->Columns->Contains("Позиция")) {
                    int idx = dataGridPlayers->Columns["Позиция"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    array<String^>^ positions = {
                        "Вратарь",
                        "Нападающий",
                        "Центральный защитник",
                        "Левый защитник",
                        "Правый защитник",
                        "Полузащитник"
                    };

                    combo->Items->AddRange(positions);
                    combo->DataPropertyName = "Позиция";
                    combo->HeaderText = "Позиция";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 150;

                    dataGridPlayers->Columns->Remove("Позиция");
                    dataGridPlayers->Columns->Insert(idx, combo);
                }

                if (dataGridPlayers->Columns->Contains("id"))
                    dataGridPlayers->Columns["id"]->ReadOnly = true;

                UpdateStats();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки игроков: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void LoadStats()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                daStats = gcnew OdbcDataAdapter("SELECT s.id, p.\"Имя\" as \"Игрок\", s.\"матчи\", s.\"голы\", s.\"голевые передачи\" "
                    "FROM \"Statistika\" s "
                    "LEFT JOIN \"Player\" p ON s.id = p.id "
                    "ORDER BY s.id;", conn);
                dtStats = gcnew DataTable();
                daStats->Fill(dtStats);

                dataGridStats->DataSource = nullptr;
                dataGridStats->Columns->Clear();
                dataGridStats->DataSource = dtStats;

                // Выпадающий список для игроков в статистике
                if (dataGridStats->Columns->Contains("Игрок")) {
                    int idx = dataGridStats->Columns["Игрок"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    // Загружаем кэш игроков
                    LoadPlayersCache();

                    combo->DataSource = playersCache;
                    combo->DisplayMember = "Имя";
                    combo->ValueMember = "id";
                    combo->DataPropertyName = "id";  // Связываем с ID игрока
                    combo->HeaderText = "Игрок";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 150;

                    dataGridStats->Columns->Remove("Игрок");
                    dataGridStats->Columns->Insert(idx, combo);
                }

                UpdateStats();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки статистики: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void LoadGoalkeepers()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                OdbcDataAdapter^ da = gcnew OdbcDataAdapter("SELECT g.id, p.\"Имя\" as \"Игрок\", g.\"матчи\", g.\"пропущенные мячи\" "
                    "FROM \"Statgp\" g "
                    "LEFT JOIN \"Player\" p ON g.id = p.id "
                    "ORDER BY g.id;", conn);
                DataTable^ dt = gcnew DataTable();
                da->Fill(dt);

                dataGridStats->DataSource = nullptr;
                dataGridStats->Columns->Clear();
                dataGridStats->DataSource = dt;

                // Выпадающий список для вратарей
                if (dataGridStats->Columns->Contains("Игрок")) {
                    int idx = dataGridStats->Columns["Игрок"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    // Загружаем кэш игроков
                    LoadPlayersCache();

                    combo->DataSource = playersCache;
                    combo->DisplayMember = "Имя";
                    combo->ValueMember = "id";
                    combo->DataPropertyName = "id";  // Связываем с ID игрока
                    combo->HeaderText = "Игрок";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 150;

                    dataGridStats->Columns->Remove("Игрок");
                    dataGridStats->Columns->Insert(idx, combo);
                }

                UpdateStats();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки вратарей: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void LoadMatches()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                matchesAdapter = gcnew OdbcDataAdapter("SELECT id, date, hause, gast, goalhause, goalgast FROM \"igri\" ORDER BY id;", conn);
                matchesTable = gcnew DataTable();
                matchesAdapter->Fill(matchesTable);

                dataGridMatches->DataSource = nullptr;
                dataGridMatches->Columns->Clear();
                dataGridMatches->DataSource = matchesTable;

                if (dataGridMatches->Columns->Contains("id")) {
                    dataGridMatches->Columns["id"]->ReadOnly = true;
                }

                // ДОБАВЛЯЕМ ВЫБОР ДАТЫ ДЛЯ КОЛОНКИ DATE
                if (dataGridMatches->Columns->Contains("date")) {
                    int idx = dataGridMatches->Columns["date"]->Index;
                    DataGridViewColumn^ col = dataGridMatches->Columns["date"];
                    col->DefaultCellStyle->Format = "dd.MM.yyyy";

                    // Добавляем обработчик клика для выбора даты
                    dataGridMatches->CellClick += gcnew DataGridViewCellEventHandler(this, &AdminPanel::MatchesGrid_CellClick);
                }

                UpdateStats();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки матчей: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void MatchesGrid_CellClick(Object^ sender, DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex < 0 || e->ColumnIndex < 0) return;

            DataGridView^ dgv = safe_cast<DataGridView^>(sender);
            DataGridViewColumn^ col = dgv->Columns[e->ColumnIndex];

            if (col->Name == "date") {
                ShowDatePicker(dgv, dgv->Rows[e->RowIndex]->Cells[e->ColumnIndex]);
            }
        }

        void LoadMatchEvents()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                matchEventsAdapter = gcnew OdbcDataAdapter(
                    "SELECT me.event_id, me.match_id, m.hause || ' vs ' || m.gast as \"Матч\", "
                    "me.player_name as \"Игрок\", me.event_type as \"Событие\", "
                    "me.minute as \"Минута\", me.details as \"Детали\" "
                    "FROM matchevents me "
                    "LEFT JOIN igri m ON me.match_id = m.id "
                    "ORDER BY me.match_id, me.minute;", conn);
                matchEventsTable = gcnew DataTable();
                matchEventsAdapter->Fill(matchEventsTable);

                dataGridMatchEvents->DataSource = nullptr;
                dataGridMatchEvents->Columns->Clear();
                dataGridMatchEvents->DataSource = matchEventsTable;

                // Загружаем кэш данных для выпадающих списков
                LoadPlayersCache();
                LoadMatchesCache();

                // Выпадающий список для матчей
                if (dataGridMatchEvents->Columns->Contains("Матч")) {
                    int idx = dataGridMatchEvents->Columns["Матч"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    combo->DataSource = matchesCache;
                    combo->DisplayMember = "MatchInfo";
                    combo->ValueMember = "id";
                    combo->DataPropertyName = "match_id";
                    combo->HeaderText = "Матч";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 200;

                    dataGridMatchEvents->Columns->Remove("Матч");
                    dataGridMatchEvents->Columns->Insert(idx, combo);
                }

                // Выпадающий список для игроков
                if (dataGridMatchEvents->Columns->Contains("Игрок")) {
                    int idx = dataGridMatchEvents->Columns["Игрок"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    combo->DataSource = playersCache;
                    combo->DisplayMember = "Имя";
                    combo->ValueMember = "Имя";  // Используем имя как значение
                    combo->DataPropertyName = "Игрок";  // Связываем с колонкой "Игрок"
                    combo->HeaderText = "Игрок";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 150;

                    dataGridMatchEvents->Columns->Remove("Игрок");
                    dataGridMatchEvents->Columns->Insert(idx, combo);
                }

                // Выпадающий список для типов событий
                if (dataGridMatchEvents->Columns->Contains("Событие")) {
                    int idx = dataGridMatchEvents->Columns["Событие"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    array<String^>^ eventTypes = {
                        "Гол", "Пасс", "Желтая карточка", "Красная карточка",
                        "Замена", "Удар", "Фол", "Угловой", "Штрафной", "Пенальти"
                    };

                    combo->Items->AddRange(eventTypes);
                    combo->DataPropertyName = "Событие";
                    combo->HeaderText = "Событие";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 150;

                    dataGridMatchEvents->Columns->Remove("Событие");
                    dataGridMatchEvents->Columns->Insert(idx, combo);
                }

                UpdateStats();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки событий матчей: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void LoadLineups()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                lineupsAdapter = gcnew OdbcDataAdapter(
                    "SELECT l.lineup_id, l.match_id, m.hause || ' vs ' || m.gast as \"Матч\", "
                    "l.player_name as \"Игрок\", l.position as \"Позиция\", "
                    "CASE WHEN l.is_starter THEN 'Основной' ELSE 'Запасной' END as \"Статус\" "
                    "FROM matchlineups l "
                    "LEFT JOIN igri m ON l.match_id = m.id "
                    "ORDER BY l.match_id, l.is_starter DESC;", conn);
                lineupsTable = gcnew DataTable();
                lineupsAdapter->Fill(lineupsTable);

                dataGridLineups->DataSource = nullptr;
                dataGridLineups->Columns->Clear();
                dataGridLineups->DataSource = lineupsTable;

                // Загружаем кэш данных для выпадающих списков
                LoadPlayersCache();
                LoadMatchesCache();

                // Выпадающий список для матчей
                if (dataGridLineups->Columns->Contains("Матч")) {
                    int idx = dataGridLineups->Columns["Матч"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    combo->DataSource = matchesCache;
                    combo->DisplayMember = "MatchInfo";
                    combo->ValueMember = "id";
                    combo->DataPropertyName = "match_id";
                    combo->HeaderText = "Матч";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 200;

                    dataGridLineups->Columns->Remove("Матч");
                    dataGridLineups->Columns->Insert(idx, combo);
                }

                // Выпадающий список для игроков
                if (dataGridLineups->Columns->Contains("Игрок")) {
                    int idx = dataGridLineups->Columns["Игрок"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    combo->DataSource = playersCache;
                    combo->DisplayMember = "Имя";
                    combo->ValueMember = "Имя";  // Используем имя как значение
                    combo->DataPropertyName = "Игрок";  // Связываем с колонкой "Игрок"
                    combo->HeaderText = "Игрок";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 150;

                    dataGridLineups->Columns->Remove("Игрок");
                    dataGridLineups->Columns->Insert(idx, combo);
                }

                // Выпадающий список для позиций
                if (dataGridLineups->Columns->Contains("Позиция")) {
                    int idx = dataGridLineups->Columns["Позиция"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    array<String^>^ positions = {
                        "Вратарь", "Центральный защитник", "Левый защитник",
                        "Правый защитник", "Опорный полузащитник", "Центральный полузащитник",
                        "Атакующий полузащитник", "Левый вингер", "Правый вингер", "Нападающий"
                    };

                    combo->Items->AddRange(positions);
                    combo->DataPropertyName = "Позиция";
                    combo->HeaderText = "Позиция";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 180;

                    dataGridLineups->Columns->Remove("Позиция");
                    dataGridLineups->Columns->Insert(idx, combo);
                }

                // Выпадающий список для статуса
                if (dataGridLineups->Columns->Contains("Статус")) {
                    int idx = dataGridLineups->Columns["Статус"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    array<String^>^ statuses = { "Основной", "Запасной" };

                    combo->Items->AddRange(statuses);
                    combo->DataPropertyName = "Статус";
                    combo->HeaderText = "Статус";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;

                    dataGridLineups->Columns->Remove("Статус");
                    dataGridLineups->Columns->Insert(idx, combo);
                }

                UpdateStats();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки составов: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void LoadInjuries()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                injuriesAdapter = gcnew OdbcDataAdapter(
                    "SELECT injury_id, player_name as \"Игрок\", injury_type as \"Тип травмы\", "
                    "date_start as \"Начало\", date_end as \"Конец\", status as \"Статус\" "
                    "FROM injuries ORDER BY date_start DESC;", conn);
                injuriesTable = gcnew DataTable();
                injuriesAdapter->Fill(injuriesTable);

                dataGridInjuries->DataSource = nullptr;
                dataGridInjuries->Columns->Clear();
                dataGridInjuries->DataSource = injuriesTable;

                // Загружаем кэш игроков
                LoadPlayersCache();

                // Выпадающий список для игроков
                if (dataGridInjuries->Columns->Contains("Игрок")) {
                    int idx = dataGridInjuries->Columns["Игрок"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    combo->DataSource = playersCache;
                    combo->DisplayMember = "Имя";
                    combo->ValueMember = "Имя";  // Используем имя как значение
                    combo->DataPropertyName = "Игрок";  // Связываем с колонкой "Игрок"
                    combo->HeaderText = "Игрок";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 150;

                    dataGridInjuries->Columns->Remove("Игрок");
                    dataGridInjuries->Columns->Insert(idx, combo);
                }

                // Выпадающий список для типов травм
                if (dataGridInjuries->Columns->Contains("Тип травмы")) {
                    int idx = dataGridInjuries->Columns["Тип травмы"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    array<String^>^ injuryTypes = {
                        "Ушиб", "Перелом", "Растяжение", "Разрыв связки", "Вывих",
                        "Сотрясение", "Малая травма", "Другое"
                    };

                    combo->Items->AddRange(injuryTypes);
                    combo->DataPropertyName = "Тип травмы";
                    combo->HeaderText = "Тип травмы";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 150;

                    dataGridInjuries->Columns->Remove("Тип травмы");
                    dataGridInjuries->Columns->Insert(idx, combo);
                }

                // Выпадающий список для статуса
                if (dataGridInjuries->Columns->Contains("Статус")) {
                    int idx = dataGridInjuries->Columns["Статус"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    array<String^>^ statuses = { "Актуальна", "Вылечена", "В процессе" };

                    combo->Items->AddRange(statuses);
                    combo->DataPropertyName = "Статус";
                    combo->HeaderText = "Статус";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;

                    dataGridInjuries->Columns->Remove("Статус");
                    dataGridInjuries->Columns->Insert(idx, combo);
                }

                // ДОБАВЛЯЕМ ВЫБОР ДАТЫ ДЛЯ КОЛОНОК С ДАТАМИ
                if (dataGridInjuries->Columns->Contains("Начало")) {
                    int idx = dataGridInjuries->Columns["Начало"]->Index;
                    DataGridViewColumn^ col = dataGridInjuries->Columns["Начало"];
                    col->DefaultCellStyle->Format = "dd.MM.yyyy";

                    // Добавляем обработчик клика для выбора даты
                    dataGridInjuries->CellClick += gcnew DataGridViewCellEventHandler(this, &AdminPanel::InjuriesGrid_CellClick);
                }

                if (dataGridInjuries->Columns->Contains("Конец")) {
                    int idx = dataGridInjuries->Columns["Конец"]->Index;
                    DataGridViewColumn^ col = dataGridInjuries->Columns["Конец"];
                    col->DefaultCellStyle->Format = "dd.MM.yyyy";
                }

                UpdateStats();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки травм: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void InjuriesGrid_CellClick(Object^ sender, DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex < 0 || e->ColumnIndex < 0) return;

            DataGridView^ dgv = safe_cast<DataGridView^>(sender);
            DataGridViewColumn^ col = dgv->Columns[e->ColumnIndex];

            if (col->Name == "Начало" || col->Name == "Конец") {
                ShowDatePicker(dgv, dgv->Rows[e->RowIndex]->Cells[e->ColumnIndex]);
            }
        }

        void LoadTrainings()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                trainingsAdapter = gcnew OdbcDataAdapter(
                    "SELECT training_id, training_date as \"Дата\", "
                    "focus_area as \"Фокус тренировки\", participants as \"Участники\" "
                    "FROM trainings ORDER BY training_date DESC;", conn);
                trainingsTable = gcnew DataTable();
                trainingsAdapter->Fill(trainingsTable);

                dataGridTrainings->DataSource = nullptr;
                dataGridTrainings->Columns->Clear();
                dataGridTrainings->DataSource = trainingsTable;

                // Выпадающий список для фокуса тренировки
                if (dataGridTrainings->Columns->Contains("Фокус тренировки")) {
                    int idx = dataGridTrainings->Columns["Фокус тренировки"]->Index;
                    DataGridViewComboBoxColumn^ combo = gcnew DataGridViewComboBoxColumn();

                    array<String^>^ focusAreas = {
                        "Тактика", "Физподготовка", "Удары", "Пас", "Стандартные положения",
                        "Оборона", "Атака", "Игра головой", "Контратаки", "Командная игра"
                    };

                    combo->Items->AddRange(focusAreas);
                    combo->DataPropertyName = "Фокус тренировки";
                    combo->HeaderText = "Фокус тренировки";
                    combo->DisplayStyle = DataGridViewComboBoxDisplayStyle::DropDownButton;
                    combo->DisplayStyleForCurrentCellOnly = true;
                    combo->Width = 180;

                    dataGridTrainings->Columns->Remove("Фокус тренировки");
                    dataGridTrainings->Columns->Insert(idx, combo);
                }

                // ДОБАВЛЯЕМ ВЫБОР ДАТЫ ДЛЯ КОЛОНКИ ДАТЫ
                if (dataGridTrainings->Columns->Contains("Дата")) {
                    int idx = dataGridTrainings->Columns["Дата"]->Index;
                    DataGridViewColumn^ col = dataGridTrainings->Columns["Дата"];
                    col->DefaultCellStyle->Format = "dd.MM.yyyy";

                    // Добавляем обработчик клика для выбора даты
                    dataGridTrainings->CellClick += gcnew DataGridViewCellEventHandler(this, &AdminPanel::TrainingsGrid_CellClick);
                }

                UpdateStats();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки тренировок: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void TrainingsGrid_CellClick(Object^ sender, DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex < 0 || e->ColumnIndex < 0) return;

            DataGridView^ dgv = safe_cast<DataGridView^>(sender);
            DataGridViewColumn^ col = dgv->Columns[e->ColumnIndex];

            if (col->Name == "Дата") {
                ShowDatePicker(dgv, dgv->Rows[e->RowIndex]->Cells[e->ColumnIndex]);
            }
        }

        // Методы для загрузки кэша данных
        void LoadPlayersCache()
        {
            try {
                if (playersCache == nullptr || playersCache->Rows->Count == 0) {
                    OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                    OdbcDataAdapter^ da = gcnew OdbcDataAdapter("SELECT id, \"Имя\" FROM \"Player\" ORDER BY \"Имя\";", conn);
                    playersCache = gcnew DataTable();
                    da->Fill(playersCache);

                    // Добавляем пустую строку для возможности очистки выбора
                    DataRow^ emptyRow = playersCache->NewRow();
                    emptyRow["id"] = DBNull::Value;
                    emptyRow["Имя"] = "";
                    playersCache->Rows->InsertAt(emptyRow, 0);
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки списка игроков: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void LoadMatchesCache()
        {
            try {
                if (matchesCache == nullptr || matchesCache->Rows->Count == 0) {
                    OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                    OdbcDataAdapter^ da = gcnew OdbcDataAdapter(
                        "SELECT id, hause || ' vs ' || gast as \"MatchInfo\", date FROM \"igri\" ORDER BY date DESC;", conn);
                    matchesCache = gcnew DataTable();
                    da->Fill(matchesCache);

                    // Добавляем пустую строку для возможности очистки выбора
                    DataRow^ emptyRow = matchesCache->NewRow();
                    emptyRow["id"] = DBNull::Value;
                    emptyRow["MatchInfo"] = "";
                    matchesCache->Rows->InsertAt(emptyRow, 0);
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки списка матчей: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

    private:
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->Size = System::Drawing::Size(1300, 900);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->Text = L"Админ-панель футбольного клуба - Modern UI";
            this->MinimumSize = System::Drawing::Size(1100, 750);
            this->Padding = System::Windows::Forms::Padding(10);

            // Create panels
            this->headerPanel = gcnew Panel();
            this->mainPanel = gcnew Panel();
            this->footerPanel = gcnew Panel();
            this->contentPanel = gcnew Panel();
            this->statsPanel = gcnew Panel();
            this->lblTitle = gcnew Label();
            this->lblSubtitle = gcnew Label();
            this->logoPicture = gcnew PictureBox();
            this->lblTotalPlayers = gcnew Label();
            this->lblTotalMatches = gcnew Label();
            this->lblActiveUsers = gcnew Label();

            // Create controls
            this->dataGridPlayers = gcnew DataGridView();
            this->dataGridStats = gcnew DataGridView();
            this->dataGridMatches = gcnew DataGridView();
            this->dataGridMatchEvents = gcnew DataGridView();
            this->dataGridLineups = gcnew DataGridView();
            this->dataGridInjuries = gcnew DataGridView();
            this->dataGridTrainings = gcnew DataGridView();

            // Основные кнопки
            this->btnQuickLoad = gcnew Button();
            this->btnQuickSave = gcnew Button();
            this->btnAddData = gcnew Button();
            this->btnBack = gcnew Button();

            // Кнопки удаления
            this->btnDeletePlayer = gcnew Button();
            this->btnDeleteMatch = gcnew Button();
            this->btnDeleteMatchEvent = gcnew Button();
            this->btnDeleteLineup = gcnew Button();
            this->btnDeleteInjury = gcnew Button();
            this->btnDeleteTraining = gcnew Button();
            this->btnBatchLineup = gcnew Button();  // Кнопка для массового ввода состава
            this->btnBatchTraining = gcnew Button();  // Новая кнопка для массового ввода участников тренировок
            this->btnBatchMatchEvents = gcnew Button();  // Новая кнопка для массового ввода событий матчей

            // Комбобокс и меню
            this->cmbTableSelector = gcnew ComboBox();
            this->mainMenu = gcnew MenuStrip();
            this->fileMenu = gcnew ToolStripMenuItem();
            this->viewMenu = gcnew ToolStripMenuItem();
            this->dataMenu = gcnew ToolStripMenuItem();
            this->toolsMenu = gcnew ToolStripMenuItem();

            // Initialize controls
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridPlayers))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridStats))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridMatches))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridMatchEvents))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridLineups))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridInjuries))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridTrainings))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->logoPicture))->BeginInit();

            this->SuspendLayout();

            // Header Panel
            this->headerPanel->Controls->Add(this->lblTitle);
            this->headerPanel->Controls->Add(this->lblSubtitle);
            this->headerPanel->Controls->Add(this->logoPicture);

            // Main Panel layout
            this->mainPanel->Controls->Add(this->contentPanel);

            // Content Panel - modern layout
            this->contentPanel->Padding = System::Windows::Forms::Padding(20);

            // Stats panel already initialized in ApplyModernDesign

            // Панель кнопок с тремя четкими рядами
            Panel^ buttonPanel = gcnew Panel();
            buttonPanel->BackColor = Color::Transparent;
            buttonPanel->Size = System::Drawing::Size(1100, 165);  // Достаточно для трех рядов с отступами
            buttonPanel->Location = Point(20, 100);
            buttonPanel->Padding = System::Windows::Forms::Padding(5, 5, 5, 5);

            // Верхний ряд - основные действия (4 кнопки)
            array<Button^>^ topRowButtons = {
                btnQuickLoad, btnQuickSave, btnAddData, btnBack
            };

            array<String^>^ topRowTexts = {
                "Загрузить", "Сохранить", "Добавить", "Назад"
            };

            int buttonX = 5;
            int buttonY = 5;
            int buttonWidth = 110;
            int buttonHeight = 40;
            int buttonSpacing = 15;  // Увеличенный отступ между кнопками

            // Верхний ряд - основные кнопки
            for (int i = 0; i < topRowButtons->Length; i++) {
                topRowButtons[i]->Text = topRowTexts[i];
                topRowButtons[i]->Location = Point(buttonX, buttonY);
                topRowButtons[i]->Size = Drawing::Size(buttonWidth, buttonHeight);

                // Применяем стиль с границами
                ApplyModernButtonStyle(topRowButtons[i], "", primaryColor);

                // Устанавливаем явные границы для четкого разделения
                topRowButtons[i]->FlatStyle = FlatStyle::Flat;
                topRowButtons[i]->FlatAppearance->BorderSize = 1;
                topRowButtons[i]->FlatAppearance->BorderColor = Color::FromArgb(200, 200, 200);

                buttonPanel->Controls->Add(topRowButtons[i]);
                buttonX += buttonWidth + buttonSpacing;
            }

            // Комбобокс для выбора таблицы - размещаем после кнопок с отступом
            cmbTableSelector->Location = Point(buttonX + 10, buttonY);  // +10 для дополнительного отступа
            cmbTableSelector->Size = Drawing::Size(200, buttonHeight);
            cmbTableSelector->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbTableSelector->Font = gcnew Drawing::Font("Segoe UI", 9);
            cmbTableSelector->Items->AddRange(gcnew array<String^> {
                "Игроки и статистика",
                    "Вратари",
                    "Матчи",
                    "События матчей",
                    "Составы",
                    "Травмы",
                    "Тренировки"
            });
            cmbTableSelector->SelectedIndex = 0;

            // Стилизация комбобокса с видимыми границами
            cmbTableSelector->BackColor = cardColor;
            cmbTableSelector->ForeColor = darkColor;
            cmbTableSelector->FlatStyle = FlatStyle::Flat;
            cmbTableSelector->DropDownHeight = 200;

            buttonPanel->Controls->Add(cmbTableSelector);

            // Средний ряд - кнопки удаления (6 кнопок)
            buttonX = 5;
            buttonY += buttonHeight + 12;  // Увеличенный вертикальный отступ между рядами

            array<Button^>^ middleRowButtons = {
                btnDeletePlayer, btnDeleteMatch, btnDeleteMatchEvent,
                btnDeleteLineup, btnDeleteInjury, btnDeleteTraining
            };

            array<String^>^ middleRowTexts = {
                "Игрока", "Матч", "Событие",
                "Состав", "Травму", "Тренировку"
            };

            // Средний ряд - кнопки удаления
            for (int i = 0; i < middleRowButtons->Length; i++) {
                middleRowButtons[i]->Text = middleRowTexts[i];
                middleRowButtons[i]->Location = Point(buttonX, buttonY);
                middleRowButtons[i]->Size = Drawing::Size(buttonWidth, buttonHeight);

                // Применяем стиль с границами
                ApplyModernButtonStyle(middleRowButtons[i], "", accentColor);

                // Устанавливаем явные границы для четкого разделения
                middleRowButtons[i]->FlatStyle = FlatStyle::Flat;
                middleRowButtons[i]->FlatAppearance->BorderSize = 1;
                middleRowButtons[i]->FlatAppearance->BorderColor = Color::FromArgb(200, 200, 200);

                buttonPanel->Controls->Add(middleRowButtons[i]);
                buttonX += buttonWidth + buttonSpacing;
            }

            // Нижний ряд - кнопки массового ввода (3 кнопки)
            buttonX = 5;
            buttonY += buttonHeight + 12;  // Увеличенный вертикальный отступ между рядами

            array<Button^>^ bottomRowButtons = {
                btnBatchLineup, btnBatchTraining, btnBatchMatchEvents
            };

            array<String^>^ bottomRowTexts = {
                "Массовый состав", "Участники тренировки", "События матча"
            };

            // Увеличиваем ширину для длинного текста
            int wideButtonWidth = 140;

            for (int i = 0; i < bottomRowButtons->Length; i++) {
                bottomRowButtons[i]->Text = bottomRowTexts[i];
                bottomRowButtons[i]->Location = Point(buttonX, buttonY);
                bottomRowButtons[i]->Size = Drawing::Size(wideButtonWidth, buttonHeight);

                // Разные цвета для разных типов кнопок
                if (i == 0) {
                    ApplyModernButtonStyle(bottomRowButtons[i], "", infoColor);  // Кнопка составов
                }
                else if (i == 1) {
                    ApplyModernButtonStyle(bottomRowButtons[i], "", Color::FromArgb(155, 89, 182));  // Кнопка тренировок
                }
                else {
                    ApplyModernButtonStyle(bottomRowButtons[i], "", warningColor);  // Кнопка событий
                }

                // Устанавливаем явные границы для четкого разделения
                bottomRowButtons[i]->FlatStyle = FlatStyle::Flat;
                bottomRowButtons[i]->FlatAppearance->BorderSize = 1;
                bottomRowButtons[i]->FlatAppearance->BorderColor = Color::FromArgb(200, 200, 200);

                buttonPanel->Controls->Add(bottomRowButtons[i]);
                buttonX += wideButtonWidth + buttonSpacing;
            }

            // DataGridViews с увеличенным пространством (с учетом трех рядов кнопок)
            int gridStartY = 280;  // Увеличенный отступ сверху для DataGridViews

            this->dataGridPlayers->Location = Point(20, gridStartY);
            this->dataGridPlayers->Size = System::Drawing::Size(540, 500);

            this->dataGridStats->Location = Point(580, gridStartY);
            this->dataGridStats->Size = System::Drawing::Size(540, 500);

            this->dataGridMatches->Location = Point(20, gridStartY);
            this->dataGridMatches->Size = System::Drawing::Size(1100, 500);
            this->dataGridMatches->Visible = false;

            this->dataGridMatchEvents->Location = Point(20, gridStartY);
            this->dataGridMatchEvents->Size = System::Drawing::Size(1100, 500);
            this->dataGridMatchEvents->Visible = false;

            this->dataGridLineups->Location = Point(20, gridStartY);
            this->dataGridLineups->Size = System::Drawing::Size(1100, 500);
            this->dataGridLineups->Visible = false;

            this->dataGridInjuries->Location = Point(20, gridStartY);
            this->dataGridInjuries->Size = System::Drawing::Size(1100, 500);
            this->dataGridInjuries->Visible = false;

            this->dataGridTrainings->Location = Point(20, gridStartY);
            this->dataGridTrainings->Size = System::Drawing::Size(1100, 500);
            this->dataGridTrainings->Visible = false;

            // Add controls to content panel
            this->contentPanel->Controls->Add(statsPanel);
            this->contentPanel->Controls->Add(buttonPanel);
            this->contentPanel->Controls->Add(dataGridPlayers);
            this->contentPanel->Controls->Add(dataGridStats);
            this->contentPanel->Controls->Add(dataGridMatches);
            this->contentPanel->Controls->Add(dataGridMatchEvents);
            this->contentPanel->Controls->Add(dataGridLineups);
            this->contentPanel->Controls->Add(dataGridInjuries);
            this->contentPanel->Controls->Add(dataGridTrainings);

            // Footer Panel
            Label^ footerLabel = gcnew Label();
            footerLabel->Text = "Футбольный клуб ВЯТГу © 2025 - Современная админ-панель v3.0 🚀";
            footerLabel->ForeColor = Color::White;
            footerLabel->Font = gcnew System::Drawing::Font("Segoe UI", 9, FontStyle::Italic);
            footerLabel->AutoSize = true;
            footerLabel->Location = Point(20, 8);
            footerPanel->Controls->Add(footerLabel);

            // Настройка главного меню
            ConfigureMainMenu();

            // Add panels to form
            this->Controls->Add(mainMenu);
            this->Controls->Add(mainPanel);
            this->Controls->Add(headerPanel);
            this->Controls->Add(footerPanel);

            // Event handlers
            this->btnQuickLoad->Click += gcnew System::EventHandler(this, &AdminPanel::btnQuickLoad_Click);
            this->btnQuickSave->Click += gcnew System::EventHandler(this, &AdminPanel::btnQuickSave_Click);
            this->btnAddData->Click += gcnew System::EventHandler(this, &AdminPanel::btnAddData_Click);
            this->btnBack->Click += gcnew System::EventHandler(this, &AdminPanel::btnBack_Click);

            // Кнопки удаления
            this->btnDeletePlayer->Click += gcnew System::EventHandler(this, &AdminPanel::btnDeletePlayer_Click);
            this->btnDeleteMatch->Click += gcnew System::EventHandler(this, &AdminPanel::btnDeleteMatch_Click);
            this->btnDeleteMatchEvent->Click += gcnew System::EventHandler(this, &AdminPanel::btnDeleteMatchEvent_Click);
            this->btnDeleteLineup->Click += gcnew System::EventHandler(this, &AdminPanel::btnDeleteLineup_Click);
            this->btnDeleteInjury->Click += gcnew System::EventHandler(this, &AdminPanel::btnDeleteInjury_Click);
            this->btnDeleteTraining->Click += gcnew System::EventHandler(this, &AdminPanel::btnDeleteTraining_Click);

            // Кнопки массового ввода
            this->btnBatchLineup->Click += gcnew System::EventHandler(this, &AdminPanel::btnBatchLineup_Click);
            this->btnBatchTraining->Click += gcnew System::EventHandler(this, &AdminPanel::btnBatchTraining_Click);
            this->btnBatchMatchEvents->Click += gcnew System::EventHandler(this, &AdminPanel::btnBatchMatchEvents_Click);  // Новый обработчик

            // Комбобокс
            this->cmbTableSelector->SelectedIndexChanged += gcnew System::EventHandler(this, &AdminPanel::cmbTableSelector_SelectedIndexChanged);

            // Обработчики двойного клика для просмотра деталей
            this->dataGridTrainings->CellDoubleClick += gcnew DataGridViewCellEventHandler(this, &AdminPanel::dataGridTrainings_CellDoubleClick);
            this->dataGridLineups->CellDoubleClick += gcnew DataGridViewCellEventHandler(this, &AdminPanel::dataGridLineups_CellDoubleClick);
            this->dataGridMatches->CellDoubleClick += gcnew DataGridViewCellEventHandler(this, &AdminPanel::dataGridMatches_CellDoubleClick);
            this->dataGridMatchEvents->CellDoubleClick += gcnew DataGridViewCellEventHandler(this, &AdminPanel::dataGridMatchEvents_CellDoubleClick); // НОВЫЙ ОБРАБОТЧИК

            this->Load += gcnew System::EventHandler(this, &AdminPanel::AdminPanel_Load);
            this->dataGridPlayers->DataError += gcnew DataGridViewDataErrorEventHandler(this, &AdminPanel::dataGrid_DataError);
            this->dataGridStats->DataError += gcnew DataGridViewDataErrorEventHandler(this, &AdminPanel::dataGrid_DataError);
            this->dataGridMatches->DataError += gcnew DataGridViewDataErrorEventHandler(this, &AdminPanel::dataGrid_DataError);
            this->dataGridMatchEvents->DataError += gcnew DataGridViewDataErrorEventHandler(this, &AdminPanel::dataGrid_DataError);
            this->dataGridLineups->DataError += gcnew DataGridViewDataErrorEventHandler(this, &AdminPanel::dataGrid_DataError);
            this->dataGridInjuries->DataError += gcnew DataGridViewDataErrorEventHandler(this, &AdminPanel::dataGrid_DataError);
            this->dataGridTrainings->DataError += gcnew DataGridViewDataErrorEventHandler(this, &AdminPanel::dataGrid_DataError);

            this->dataGridPlayers->DataBindingComplete += gcnew DataGridViewBindingCompleteEventHandler(this, &AdminPanel::dataGridPlayers_DataBindingComplete);
            this->dataGridStats->DataBindingComplete += gcnew DataGridViewBindingCompleteEventHandler(this, &AdminPanel::dataGridStats_DataBindingComplete);
            this->dataGridMatches->DataBindingComplete += gcnew DataGridViewBindingCompleteEventHandler(this, &AdminPanel::dataGridMatches_DataBindingComplete);
            this->dataGridMatchEvents->DataBindingComplete += gcnew DataGridViewBindingCompleteEventHandler(this, &AdminPanel::dataGridMatchEvents_DataBindingComplete);
            this->dataGridLineups->DataBindingComplete += gcnew DataGridViewBindingCompleteEventHandler(this, &AdminPanel::dataGridLineups_DataBindingComplete);
            this->dataGridInjuries->DataBindingComplete += gcnew DataGridViewBindingCompleteEventHandler(this, &AdminPanel::dataGridInjuries_DataBindingComplete);
            this->dataGridTrainings->DataBindingComplete += gcnew DataGridViewBindingCompleteEventHandler(this, &AdminPanel::dataGridTrainings_DataBindingComplete);

            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridPlayers))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridStats))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridMatches))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridMatchEvents))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridLineups))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridInjuries))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridTrainings))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->logoPicture))->EndInit();

            this->ResumeLayout(false);
        }

        void ConfigureMainMenu()
        {
            // Меню Файл
            array<ToolStripMenuItem^>^ fileItems = {
                gcnew ToolStripMenuItem("Экспорт данных", nullptr, gcnew EventHandler(this, &AdminPanel::ExportData)),
                gcnew ToolStripMenuItem("Импорт данных", nullptr, gcnew EventHandler(this, &AdminPanel::ImportData)),
                gcnew ToolStripMenuItem("-"),
                gcnew ToolStripMenuItem("Выход", nullptr, gcnew EventHandler(this, &AdminPanel::ExitApp))
            };
            fileMenu->DropDownItems->AddRange(fileItems);
            fileMenu->Text = "Файл";

            // Меню Вид
            array<ToolStripMenuItem^>^ viewItems = {
                gcnew ToolStripMenuItem("Игроки и статистика", nullptr, gcnew EventHandler(this, &AdminPanel::ShowPlayersStats)),
                gcnew ToolStripMenuItem("Вратари", nullptr, gcnew EventHandler(this, &AdminPanel::ShowGoalkeepers)),
                gcnew ToolStripMenuItem("Матчи", nullptr, gcnew EventHandler(this, &AdminPanel::ShowMatches)),
                gcnew ToolStripMenuItem("События матчей", nullptr, gcnew EventHandler(this, &AdminPanel::ShowMatchEvents)),
                gcnew ToolStripMenuItem("Составы", nullptr, gcnew EventHandler(this, &AdminPanel::ShowLineups)),
                gcnew ToolStripMenuItem("Травмы", nullptr, gcnew EventHandler(this, &AdminPanel::ShowInjuries)),
                gcnew ToolStripMenuItem("Тренировки", nullptr, gcnew EventHandler(this, &AdminPanel::ShowTrainings))
            };
            viewMenu->DropDownItems->AddRange(viewItems);
            viewMenu->Text = "Вид";

            // Меню Данные
            array<ToolStripMenuItem^>^ dataItems = {
                gcnew ToolStripMenuItem("Сохранить все", nullptr, gcnew EventHandler(this, &AdminPanel::SaveAllData)),
                gcnew ToolStripMenuItem("Загрузить все", nullptr, gcnew EventHandler(this, &AdminPanel::LoadAllData)),
                gcnew ToolStripMenuItem("-"),
                gcnew ToolStripMenuItem("Быстрый состав", nullptr, gcnew EventHandler(this, &AdminPanel::CreateQuickLineup)),
                gcnew ToolStripMenuItem("Участники тренировки", nullptr, gcnew EventHandler(this, &AdminPanel::CreateQuickTrainingParticipants)),
                gcnew ToolStripMenuItem("События матча", nullptr, gcnew EventHandler(this, &AdminPanel::CreateQuickMatchEvents)), // Новый пункт
                gcnew ToolStripMenuItem("Очистить кэш", nullptr, gcnew EventHandler(this, &AdminPanel::ClearCache))
            };
            dataMenu->DropDownItems->AddRange(dataItems);
            dataMenu->Text = "Данные";

            // Меню Инструменты
            array<ToolStripMenuItem^>^ toolsItems = {
                gcnew ToolStripMenuItem("Статистика команды", nullptr, gcnew EventHandler(this, &AdminPanel::ShowTeamStats)),
                gcnew ToolStripMenuItem("Анализ эффективности", nullptr, gcnew EventHandler(this, &AdminPanel::ShowEfficiencyAnalysis)),
                gcnew ToolStripMenuItem("Отчет по травмам", nullptr, gcnew EventHandler(this, &AdminPanel::ShowInjuryReport))
            };
            toolsMenu->DropDownItems->AddRange(toolsItems);
            toolsMenu->Text = "Инструменты";

            // Добавляем меню в главную полосу
            mainMenu->Items->AddRange(gcnew array<ToolStripItem^> {
                fileMenu, viewMenu, dataMenu, toolsMenu
            });

            mainMenu->BackColor = darkColor;
            mainMenu->ForeColor = Color::White;
            mainMenu->Dock = DockStyle::Top;
        }

        // НОВЫЕ МЕТОДЫ ДЛЯ ПРОСМОТРА УЧАСТНИКОВ И СОСТАВОВ
    private:
        // Обработчик двойного клика по тренировкам (просмотр участников)
        System::Void dataGridTrainings_CellDoubleClick(System::Object^ sender, DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex < 0 || e->ColumnIndex < 0) return;

            DataGridView^ dgv = safe_cast<DataGridView^>(sender);
            DataRowView^ drv = dynamic_cast<DataRowView^>(dgv->Rows[e->RowIndex]->DataBoundItem);
            if (drv == nullptr) return;

            int trainingId = Convert::ToInt32(drv->Row["training_id"]);
            String^ trainingDate = drv->Row->IsNull("Дата") ? "" : drv->Row["Дата"]->ToString();
            String^ trainingFocus = drv->Row->IsNull("Фокус тренировки") ? "" : drv->Row["Фокус тренировки"]->ToString();
            String^ trainingInfo = String::Format("{0} - {1}", trainingDate, trainingFocus);

            // Открываем форму просмотра участников
            TrainingParticipantsForm^ participantsForm = gcnew TrainingParticipantsForm(trainingId, trainingInfo, connStr);
            participantsForm->ShowDialog();
        }

        // Обработчик двойного клика по составам (просмотр состава)
        System::Void dataGridLineups_CellDoubleClick(System::Object^ sender, DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex < 0 || e->ColumnIndex < 0) return;

            DataGridView^ dgv = safe_cast<DataGridView^>(sender);
            DataRowView^ drv = dynamic_cast<DataRowView^>(dgv->Rows[e->RowIndex]->DataBoundItem);
            if (drv == nullptr) return;

            int matchId = Convert::ToInt32(drv->Row["match_id"]);
            String^ matchInfo = drv->Row["Матч"]->ToString();

            // Открываем форму просмотра состава
            MatchLineupForm^ lineupForm = gcnew MatchLineupForm(matchId, matchInfo, connStr);
            lineupForm->ShowDialog();
        }

        // Обработчик двойного клика по событиям матчей (просмотр всех событий матча)
        System::Void dataGridMatchEvents_CellDoubleClick(System::Object^ sender, DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex < 0 || e->ColumnIndex < 0) return;

            DataGridView^ dgv = safe_cast<DataGridView^>(sender);
            DataRowView^ drv = dynamic_cast<DataRowView^>(dgv->Rows[e->RowIndex]->DataBoundItem);
            if (drv == nullptr) return;

            int matchId = Convert::ToInt32(drv->Row["match_id"]);
            String^ matchInfo = drv->Row["Матч"]->ToString();

            // Открываем форму просмотра событий
            MatchEventsViewForm^ eventsForm = gcnew MatchEventsViewForm(matchId, matchInfo, connStr);
            eventsForm->ShowDialog();
        }

        // Обработчик двойного клика по матчам (просмотр состава или событий)
        System::Void dataGridMatches_CellDoubleClick(System::Object^ sender, DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex < 0 || e->ColumnIndex < 0) return;

            DataGridView^ dgv = safe_cast<DataGridView^>(sender);
            DataRowView^ drv = dynamic_cast<DataRowView^>(dgv->Rows[e->RowIndex]->DataBoundItem);
            if (drv == nullptr) return;

            int matchId = Convert::ToInt32(drv->Row["id"]);
            String^ hause = drv->Row->IsNull("hause") ? "" : drv->Row["hause"]->ToString();
            String^ gast = drv->Row->IsNull("gast") ? "" : drv->Row["gast"]->ToString();
            String^ matchInfo = String::Format("{0} vs {1}", hause, gast);

            // Диалог выбора действия
            String^ message = "Что вы хотите просмотреть для этого матча?\n\n" + matchInfo;
            String^ caption = "Выбор действия";

            System::Windows::Forms::DialogResult result = MessageBox::Show(
                message,
                caption,
                MessageBoxButtons::YesNoCancel,
                MessageBoxIcon::Question
            );

            if (result == System::Windows::Forms::DialogResult::Yes) { // Просмотр событий
                MatchEventsViewForm^ eventsForm = gcnew MatchEventsViewForm(matchId, matchInfo, connStr);
                eventsForm->ShowDialog();
            }
            else if (result == System::Windows::Forms::DialogResult::No) { // Просмотр состава
                // Проверяем, есть ли состав для этого матча
                try {
                    OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                    OdbcCommand^ cmd = gcnew OdbcCommand(
                        "SELECT COUNT(*) FROM matchlineups WHERE match_id = ?;", conn);
                    cmd->Parameters->AddWithValue("match_id", matchId);
                    conn->Open();

                    int count = Convert::ToInt32(cmd->ExecuteScalar());
                    conn->Close();

                    if (count > 0) {
                        // Открываем форму просмотра состава
                        MatchLineupForm^ lineupForm = gcnew MatchLineupForm(matchId, matchInfo, connStr);
                        lineupForm->ShowDialog();
                    }
                    else {
                        MessageBox::Show("Для этого матча еще не создан состав.", "Информация",
                            MessageBoxButtons::OK, MessageBoxIcon::Information);
                    }
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Ошибка проверки состава: " + ex->Message, "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
            // При выборе "Отмена" ничего не делаем
        }

        // НОВЫЕ МЕТОДЫ ДЛЯ УНИВЕРСАЛЬНОЙ ФОРМЫ МАССОВОГО ВВОДА
        System::Void btnBatchLineup_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dataGridLineups->Visible) {
                // Получаем выбранный матч из таблицы составов
                if (dataGridLineups->CurrentRow == nullptr || dataGridLineups->CurrentRow->IsNewRow) {
                    MessageBox::Show("Выберите матч для редактирования состава!", "Внимание",
                        MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    return;
                }

                DataRowView^ drv = dynamic_cast<DataRowView^>(dataGridLineups->CurrentRow->DataBoundItem);
                if (drv == nullptr) return;

                int matchId = Convert::ToInt32(drv->Row["match_id"]);
                String^ matchInfo = drv->Row["Матч"]->ToString();

                // Открываем универсальную форму массового ввода для составов
                BatchForm^ batchForm = gcnew BatchForm(BatchForm::FormMode::Lineup, matchId, matchInfo, connStr);
                if (batchForm->ShowDialog() == Windows::Forms::DialogResult::OK) {
                    // Обновляем данные после сохранения
                    LoadLineups();
                }
            }
            else if (dataGridMatches->Visible) {
                // Если открыта таблица матчей, используем выбранный матч оттуда
                if (dataGridMatches->CurrentRow == nullptr || dataGridMatches->CurrentRow->IsNewRow) {
                    MessageBox::Show("Выберите матч для создания состава!", "Внимание",
                        MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    return;
                }

                DataRowView^ drv = dynamic_cast<DataRowView^>(dataGridMatches->CurrentRow->DataBoundItem);
                if (drv == nullptr) return;

                int matchId = Convert::ToInt32(drv->Row["id"]);
                String^ hause = drv->Row->IsNull("hause") ? "" : drv->Row["hause"]->ToString();
                String^ gast = drv->Row->IsNull("gast") ? "" : drv->Row["gast"]->ToString();
                String^ matchInfo = String::Format("{0} vs {1}", hause, gast);

                // Открываем универсальную форму массового ввода для составов
                BatchForm^ batchForm = gcnew BatchForm(BatchForm::FormMode::Lineup, matchId, matchInfo, connStr);
                if (batchForm->ShowDialog() == Windows::Forms::DialogResult::OK) {
                    LoadLineups();
                }
            }
            else {
                MessageBox::Show("Перейдите в раздел 'Составы' или 'Матчи' для использования этой функции!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        System::Void btnBatchTraining_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dataGridTrainings->Visible) {
                if (dataGridTrainings->CurrentRow == nullptr || dataGridTrainings->CurrentRow->IsNewRow) {
                    MessageBox::Show("Выберите тренировку для редактирования участников!", "Внимание",
                        MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    return;
                }

                DataRowView^ drv = dynamic_cast<DataRowView^>(dataGridTrainings->CurrentRow->DataBoundItem);
                if (drv == nullptr) return;

                int trainingId = Convert::ToInt32(drv->Row["training_id"]);
                String^ trainingDate = drv->Row->IsNull("Дата") ? "" : drv->Row["Дата"]->ToString();
                String^ trainingFocus = drv->Row->IsNull("Фокус тренировки") ? "" : drv->Row["Фокус тренировки"]->ToString();
                String^ trainingInfo = String::Format("{0} - {1}", trainingDate, trainingFocus);

                // Открываем универсальную форму массового ввода для тренировок
                BatchForm^ batchForm = gcnew BatchForm(BatchForm::FormMode::Training, trainingId, trainingInfo, connStr);
                if (batchForm->ShowDialog() == Windows::Forms::DialogResult::OK) {
                    LoadTrainings();
                }
            }
            else {
                MessageBox::Show("Перейдите в раздел 'Тренировки' для использования этой функции!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        // НОВЫЙ МЕТОД ДЛЯ МАССОВОГО ВВОДА СОБЫТИЙ МАТЧЕЙ
        System::Void btnBatchMatchEvents_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (dataGridMatchEvents->Visible) {
                // Если открыта таблица событий, используем выбранный матч оттуда
                if (dataGridMatchEvents->CurrentRow != nullptr && !dataGridMatchEvents->CurrentRow->IsNewRow) {
                    DataRowView^ drv = dynamic_cast<DataRowView^>(dataGridMatchEvents->CurrentRow->DataBoundItem);
                    if (drv != nullptr) {
                        int matchId = Convert::ToInt32(drv->Row["match_id"]);
                        String^ matchInfo = drv->Row["Матч"]->ToString();

                        MatchEventsBatchForm^ eventsForm = gcnew MatchEventsBatchForm(matchId, matchInfo, connStr);
                        if (eventsForm->ShowDialog() == Windows::Forms::DialogResult::OK) {
                            LoadMatchEvents(); // Обновляем данные
                        }
                        return;
                    }
                }
            }

            if (dataGridMatches->Visible) {
                // Если открыта таблица матчей, используем выбранный матч оттуда
                if (dataGridMatches->CurrentRow != nullptr && !dataGridMatches->CurrentRow->IsNewRow) {
                    DataRowView^ drv = dynamic_cast<DataRowView^>(dataGridMatches->CurrentRow->DataBoundItem);
                    if (drv != nullptr) {
                        int matchId = Convert::ToInt32(drv->Row["id"]);
                        String^ hause = drv->Row->IsNull("hause") ? "" : drv->Row["hause"]->ToString();
                        String^ gast = drv->Row->IsNull("gast") ? "" : drv->Row["gast"]->ToString();
                        String^ matchInfo = String::Format("{0} vs {1}", hause, gast);

                        MatchEventsBatchForm^ eventsForm = gcnew MatchEventsBatchForm(matchId, matchInfo, connStr);
                        if (eventsForm->ShowDialog() == Windows::Forms::DialogResult::OK) {
                            LoadMatchEvents(); // Обновляем данные
                        }
                        return;
                    }
                }
            }

            MessageBox::Show("Перейдите в раздел 'События матчей' или 'Матчи' и выберите матч для добавления событий!", "Внимание",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        System::Void CreateQuickLineup(System::Object^ sender, System::EventArgs^ e)
        {
            try {
                // Получаем последний матч для быстрого создания состава
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                OdbcCommand^ cmd = gcnew OdbcCommand(
                    "SELECT id, hause || ' vs ' || gast as match_info FROM \"igri\" ORDER BY date DESC LIMIT 1;", conn);
                conn->Open();
                OdbcDataReader^ reader = cmd->ExecuteReader();

                if (reader->Read()) {
                    int matchId = reader->GetInt32(0);
                    String^ matchInfo = reader->GetString(1);
                    reader->Close();
                    conn->Close();

                    // Используем универсальную форму
                    BatchForm^ batchForm = gcnew BatchForm(BatchForm::FormMode::Lineup, matchId, matchInfo, connStr);
                    if (batchForm->ShowDialog() == Windows::Forms::DialogResult::OK) {
                        LoadLineups();
                        // Переключаемся на вкладку составов
                        cmbTableSelector->SelectedIndex = 4; // Составы
                    }
                }
                else {
                    reader->Close();
                    conn->Close();
                    MessageBox::Show("Нет доступных матчей для создания состава!", "Внимание",
                        MessageBoxButtons::OK, MessageBoxIcon::Warning);
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка создания состава: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void CreateQuickTrainingParticipants(System::Object^ sender, System::EventArgs^ e)
        {
            try {
                // Получаем последнюю тренировку для быстрого добавления участников
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                OdbcCommand^ cmd = gcnew OdbcCommand(
                    "SELECT training_id, training_date || ' - ' || COALESCE(focus_area, '') as training_info FROM trainings ORDER BY training_date DESC LIMIT 1;", conn);
                conn->Open();
                OdbcDataReader^ reader = cmd->ExecuteReader();

                if (reader->Read()) {
                    int trainingId = reader->GetInt32(0);
                    String^ trainingInfo = reader->GetString(1);
                    reader->Close();
                    conn->Close();

                    // Используем универсальную форму для тренировок
                    BatchForm^ batchForm = gcnew BatchForm(BatchForm::FormMode::Training, trainingId, trainingInfo, connStr);
                    if (batchForm->ShowDialog() == Windows::Forms::DialogResult::OK) {
                        LoadTrainings();
                        // Переключаемся на вкладку тренировок
                        cmbTableSelector->SelectedIndex = 6; // Тренировки
                    }
                }
                else {
                    reader->Close();
                    conn->Close();
                    MessageBox::Show("Нет доступных тренировок для добавления участников!", "Внимание",
                        MessageBoxButtons::OK, MessageBoxIcon::Warning);
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка добавления участников тренировки: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        // НОВЫЙ МЕТОД ДЛЯ БЫСТРОГО СОЗДАНИЯ СОБЫТИЙ МАТЧА
        System::Void CreateQuickMatchEvents(System::Object^ sender, System::EventArgs^ e)
        {
            try {
                // Получаем последний матч для быстрого добавления событий
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                OdbcCommand^ cmd = gcnew OdbcCommand(
                    "SELECT id, hause || ' vs ' || gast as match_info FROM \"igri\" ORDER BY date DESC LIMIT 1;", conn);
                conn->Open();
                OdbcDataReader^ reader = cmd->ExecuteReader();

                if (reader->Read()) {
                    int matchId = reader->GetInt32(0);
                    String^ matchInfo = reader->GetString(1);
                    reader->Close();
                    conn->Close();

                    MatchEventsBatchForm^ eventsForm = gcnew MatchEventsBatchForm(matchId, matchInfo, connStr);
                    if (eventsForm->ShowDialog() == Windows::Forms::DialogResult::OK) {
                        LoadMatchEvents();
                        // Переключаемся на вкладку событий
                        cmbTableSelector->SelectedIndex = 3; // События матчей
                    }
                }
                else {
                    reader->Close();
                    conn->Close();
                    MessageBox::Show("Нет доступных матчей для добавления событий!", "Внимание",
                        MessageBoxButtons::OK, MessageBoxIcon::Warning);
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка создания событий матча: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        // ОСНОВНЫЕ МЕТОДЫ ОБРАБОТКИ СОБЫТИЙ
        System::Void AdminPanel_Load(System::Object^ sender, System::EventArgs^ e) {
            LoadPlayers();
            LoadStats();
        }

        System::Void dataGrid_DataError(System::Object^ sender, DataGridViewDataErrorEventArgs^ e) {
            e->ThrowException = false;
            e->Cancel = true;

            String^ errorMessage = "Ошибка ввода данных: ";

            if (e->Exception != nullptr) {
                errorMessage += e->Exception->Message;
            }
            else {
                errorMessage += "Недопустимое значение для выпадающего списка";
            }

            // Показываем сообщение только для серьезных ошибок
            if (e->Context.HasFlag(DataGridViewDataErrorContexts::Commit) ||
                e->Context.HasFlag(DataGridViewDataErrorContexts::CurrentCellChange)) {
                MessageBox::Show(errorMessage, "Ошибка данных",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
            }
        }

        System::Void cmbTableSelector_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
            switch (cmbTableSelector->SelectedIndex) {
            case 0: // Игроки и статистика
                ShowPlayersStats(sender, e);
                break;
            case 1: // Вратари
                ShowGoalkeepers(sender, e);
                break;
            case 2: // Матчи
                ShowMatches(sender, e);
                break;
            case 3: // События матчей
                ShowMatchEvents(sender, e);
                break;
            case 4: // Составы
                ShowLineups(sender, e);
                break;
            case 5: // Травмы
                ShowInjuries(sender, e);
                break;
            case 6: // Тренировки
                ShowTrainings(sender, e);
                break;
            }
        }

        System::Void btnQuickLoad_Click(System::Object^ sender, System::EventArgs^ e) {
            switch (cmbTableSelector->SelectedIndex) {
            case 0:
                LoadPlayers();
                LoadStats();
                break;
            case 1:
                LoadPlayers();
                LoadGoalkeepers();
                break;
            case 2:
                LoadMatches();
                break;
            case 3:
                LoadMatchEvents();
                break;
            case 4:
                LoadLineups();
                break;
            case 5:
                LoadInjuries();
                break;
            case 6:
                LoadTrainings();
                break;
            }
        }

        System::Void btnQuickSave_Click(System::Object^ sender, System::EventArgs^ e) {
            switch (cmbTableSelector->SelectedIndex) {
            case 0:
                btnSavePlayers_Click(sender, e);
                btnSaveStats_Click(sender, e);
                break;
            case 1:
                btnSavePlayers_Click(sender, e);
                btnSaveStats_Click(sender, e);
                break;
            case 2:
                btnSaveMatches_Click(sender, e);
                break;
            case 3:
                btnSaveMatchEvents_Click(sender, e);
                break;
            case 4:
                btnSaveLineups_Click(sender, e);
                break;
            case 5:
                btnSaveInjuries_Click(sender, e);
                break;
            case 6:
                btnSaveTrainings_Click(sender, e);
                break;
            }
        }

        // Методы отображения таблиц
        void ShowPlayersStats(Object^ sender, EventArgs^ e) {
            HideAllDataGrids();
            dataGridPlayers->Visible = true;
            dataGridStats->Visible = true;
            LoadPlayers();
            LoadStats();
        }

        void ShowGoalkeepers(Object^ sender, EventArgs^ e) {
            HideAllDataGrids();
            dataGridPlayers->Visible = true;
            dataGridStats->Visible = true;
            LoadPlayers();
            LoadGoalkeepers();
        }

        void ShowMatches(Object^ sender, EventArgs^ e) {
            HideAllDataGrids();
            dataGridMatches->Visible = true;
            dataGridMatches->BringToFront();
            LoadMatches();
        }

        void ShowMatchEvents(Object^ sender, EventArgs^ e) {
            HideAllDataGrids();
            dataGridMatchEvents->Visible = true;
            dataGridMatchEvents->BringToFront();
            LoadMatchEvents();
        }

        void ShowLineups(Object^ sender, EventArgs^ e) {
            HideAllDataGrids();
            dataGridLineups->Visible = true;
            dataGridLineups->BringToFront();
            LoadLineups();
        }

        void ShowInjuries(Object^ sender, EventArgs^ e) {
            HideAllDataGrids();
            dataGridInjuries->Visible = true;
            dataGridInjuries->BringToFront();
            LoadInjuries();
        }

        void ShowTrainings(Object^ sender, EventArgs^ e) {
            HideAllDataGrids();
            dataGridTrainings->Visible = true;
            dataGridTrainings->BringToFront();
            LoadTrainings();
        }

        void HideAllDataGrids()
        {
            dataGridPlayers->Visible = false;
            dataGridStats->Visible = false;
            dataGridMatches->Visible = false;
            dataGridMatchEvents->Visible = false;
            dataGridLineups->Visible = false;
            dataGridInjuries->Visible = false;
            dataGridTrainings->Visible = false;
        }

        // DataBindingComplete методы
        System::Void dataGridPlayers_DataBindingComplete(System::Object^ sender, DataGridViewBindingCompleteEventArgs^ e) {
            ColorRows(dataGridPlayers);
        }

        System::Void dataGridStats_DataBindingComplete(System::Object^ sender, DataGridViewBindingCompleteEventArgs^ e) {
            ColorRows(dataGridStats);
        }

        System::Void dataGridMatches_DataBindingComplete(System::Object^ sender, DataGridViewBindingCompleteEventArgs^ e) {
            ColorRows(dataGridMatches);
        }

        System::Void dataGridMatchEvents_DataBindingComplete(System::Object^ sender, DataGridViewBindingCompleteEventArgs^ e) {
            ColorRows(dataGridMatchEvents);
        }

        System::Void dataGridLineups_DataBindingComplete(System::Object^ sender, DataGridViewBindingCompleteEventArgs^ e) {
            ColorRows(dataGridLineups);
        }

        System::Void dataGridInjuries_DataBindingComplete(System::Object^ sender, DataGridViewBindingCompleteEventArgs^ e) {
            ColorRows(dataGridInjuries);
        }

        System::Void dataGridTrainings_DataBindingComplete(System::Object^ sender, DataGridViewBindingCompleteEventArgs^ e) {
            ColorRows(dataGridTrainings);
        }

        void ColorRows(DataGridView^ dgv)
        {
            try {
                for each (DataGridViewRow ^ row in dgv->Rows) {
                    if (row->IsNewRow) continue;
                    DataRowView^ drv = dynamic_cast<DataRowView^>(row->DataBoundItem);
                    if (drv == nullptr) {
                        row->DefaultCellStyle->BackColor = Color::White;
                        continue;
                    }
                    DataRow^ dr = drv->Row;
                    if (dr->RowState == DataRowState::Added)
                        row->DefaultCellStyle->BackColor = Color::FromArgb(204, 255, 204);
                    else if (dr->RowState == DataRowState::Modified)
                        row->DefaultCellStyle->BackColor = Color::FromArgb(255, 255, 204);
                    else
                        row->DefaultCellStyle->BackColor = Color::White;
                }
            }
            catch (Exception^ ex) {
                // Игнорируем ошибки окрашивания
            }
        }

        int GetNextAvailableId(const String^ tableName, OdbcConnection^ conn)
        {
            try {
                OdbcCommand^ cmd = gcnew OdbcCommand("SELECT id FROM \"" + tableName + "\" ORDER BY id;", conn);
                OdbcDataReader^ r = cmd->ExecuteReader();
                int expected = 1;
                while (r->Read()) {
                    if (r->IsDBNull(0)) continue;
                    int current = r->GetInt32(0);
                    if (current > expected) break;
                    if (current == expected) expected++;
                }
                r->Close();
                return expected;
            }
            catch (...) {
                return 1;
            }
        }

        System::Void btnBack_Click(System::Object^ sender, System::EventArgs^ e) {
            this->Hide();
            if (MyForm != nullptr)
                MyForm->Show();
        }

        System::Void btnAddData_Click(System::Object^ sender, System::EventArgs^ e) {
            dobavit^ addForm = gcnew dobavit(this);
            addForm->ShowDialog();
        }

        // МЕТОДЫ СОХРАНЕНИЯ ДАННЫХ
        System::Void btnSavePlayers_Click(System::Object^ sender, System::EventArgs^ e) {
            if (dtPlayers == nullptr) {
                MessageBox::Show("Нет данных игроков.", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            OdbcConnection^ conn = nullptr;
            try {
                conn = gcnew OdbcConnection(connStr);
                conn->Open();

                System::Collections::Generic::List<int>^ usedIds = gcnew System::Collections::Generic::List<int>();
                OdbcCommand^ cmdIds = gcnew OdbcCommand("SELECT id FROM \"Player\" ORDER BY id;", conn);
                OdbcDataReader^ reader = cmdIds->ExecuteReader();
                while (reader->Read()) {
                    usedIds->Add(reader->GetInt32(0));
                }
                reader->Close();

                int nextId = 1;

                OdbcCommand^ insertCmd = gcnew OdbcCommand("INSERT INTO \"Player\" (id, \"Имя\", \"Позиция\") VALUES (?, ?, ?);", conn);
                insertCmd->Parameters->Add(gcnew OdbcParameter());
                insertCmd->Parameters->Add(gcnew OdbcParameter());
                insertCmd->Parameters->Add(gcnew OdbcParameter());

                OdbcCommand^ updateCmd = gcnew OdbcCommand("UPDATE \"Player\" SET \"Имя\"=?, \"Позиция\"=? WHERE id=?;", conn);
                updateCmd->Parameters->Add(gcnew OdbcParameter());
                updateCmd->Parameters->Add(gcnew OdbcParameter());
                updateCmd->Parameters->Add(gcnew OdbcParameter());

                OdbcCommand^ deleteCmd = gcnew OdbcCommand("DELETE FROM \"Player\" WHERE id=?;", conn);
                deleteCmd->Parameters->Add(gcnew OdbcParameter());

                // Команды для работы с таблицей вратарей
                OdbcCommand^ checkGoalkeeperCmd = gcnew OdbcCommand("SELECT COUNT(*) FROM \"Statgp\" WHERE id=?;", conn);
                checkGoalkeeperCmd->Parameters->Add(gcnew OdbcParameter());

                OdbcCommand^ insertGoalkeeperCmd = gcnew OdbcCommand("INSERT INTO \"Statgp\" (id, \"матчи\", \"пропущенные мячи\") VALUES (?, 0, 0);", conn);
                insertGoalkeeperCmd->Parameters->Add(gcnew OdbcParameter());

                OdbcCommand^ deleteGoalkeeperCmd = gcnew OdbcCommand("DELETE FROM \"Statgp\" WHERE id=?;", conn);
                deleteGoalkeeperCmd->Parameters->Add(gcnew OdbcParameter());

                array<DataRow^>^ rows = gcnew array<DataRow^>(dtPlayers->Rows->Count);
                dtPlayers->Rows->CopyTo(rows, 0);

                for each (DataRow ^ row in rows) {
                    if (row->RowState == DataRowState::Added) {
                        while (usedIds->Contains(nextId)) nextId++;

                        insertCmd->Parameters[0]->Value = nextId;
                        insertCmd->Parameters[1]->Value = row->IsNull("Имя") ? (Object^)DBNull::Value : row["Имя"];
                        insertCmd->Parameters[2]->Value = row->IsNull("Позиция") ? (Object^)DBNull::Value : row["Позиция"];
                        insertCmd->ExecuteNonQuery();

                        // АВТОМАТИЧЕСКОЕ ДОБАВЛЕНИЕ ВРАТАРЯ В ТАБЛИЦУ ВРАТАРЕЙ
                        if (!row->IsNull("Позиция") && row["Позиция"]->ToString() == "Вратарь") {
                            insertGoalkeeperCmd->Parameters[0]->Value = nextId;
                            insertGoalkeeperCmd->ExecuteNonQuery();
                        }

                        row["id"] = nextId;
                        row->AcceptChanges();
                        usedIds->Add(nextId);
                        nextId++;
                    }
                    else if (row->RowState == DataRowState::Modified) {
                        Object^ originalId = row["id", DataRowVersion::Original];
                        String^ originalPosition = row->HasVersion(DataRowVersion::Original) ?
                            (row["Позиция", DataRowVersion::Original] == DBNull::Value ? "" : row["Позиция", DataRowVersion::Original]->ToString()) : "";
                        String^ newPosition = row->IsNull("Позиция") ? "" : row["Позиция"]->ToString();

                        updateCmd->Parameters[0]->Value = row->IsNull("Имя") ? (Object^)DBNull::Value : row["Имя"];
                        updateCmd->Parameters[1]->Value = row->IsNull("Позиция") ? (Object^)DBNull::Value : row["Позиция"];
                        updateCmd->Parameters[2]->Value = row["id"];
                        updateCmd->ExecuteNonQuery();

                        // ОБРАБОТКА ИЗМЕНЕНИЯ ПОЗИЦИИ НА ВРАТАРЯ
                        if (originalPosition != "Вратарь" && newPosition == "Вратарь") {
                            // Игрок стал вратарем - добавляем в таблицу вратарей
                            checkGoalkeeperCmd->Parameters[0]->Value = row["id"];
                            int count = Convert::ToInt32(checkGoalkeeperCmd->ExecuteScalar());
                            if (count == 0) {
                                insertGoalkeeperCmd->Parameters[0]->Value = row["id"];
                                insertGoalkeeperCmd->ExecuteNonQuery();
                            }
                        }
                        // ОБРАБОТКА ИЗМЕНЕНИЯ ПОЗИЦИИ С ВРАТАРЯ НА ДРУГУЮ
                        else if (originalPosition == "Вратарь" && newPosition != "Вратарь") {
                            // Игрок больше не вратарь - удаляем из таблицы вратарей
                            deleteGoalkeeperCmd->Parameters[0]->Value = row["id"];
                            deleteGoalkeeperCmd->ExecuteNonQuery();
                        }

                        row->AcceptChanges();
                    }
                }

                // Обработка удаленных строк
                array<DataRow^>^ delRows = dtPlayers->Select(nullptr, nullptr, DataViewRowState::Deleted);
                for each (DataRow ^ drow in delRows) {
                    Object^ origId = drow["id", DataRowVersion::Original];
                    Object^ origPosition = drow->HasVersion(DataRowVersion::Original) ?
                        drow["Позиция", DataRowVersion::Original] : nullptr;

                    // УДАЛЕНИЕ ВРАТАРЯ ИЗ ТАБЛИЦЫ ВРАТАРЕЙ ПРИ УДАЛЕНИИ ИГРОКА
                    if (origPosition != nullptr && origPosition->ToString() == "Вратарь") {
                        deleteGoalkeeperCmd->Parameters[0]->Value = origId;
                        deleteGoalkeeperCmd->ExecuteNonQuery();
                    }

                    deleteCmd->Parameters[0]->Value = origId;
                    deleteCmd->ExecuteNonQuery();
                    drow->AcceptChanges();
                }

                conn->Close();
                LoadPlayers();
                LoadStats();
                MessageBox::Show("Игроки успешно сохранены!", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                try { if (conn != nullptr) conn->Close(); }
                catch (...) {}
                MessageBox::Show("Ошибка сохранения игроков: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnSaveStats_Click(System::Object^ sender, System::EventArgs^ e) {
            // Проверяем, что dataGridStats имеет данные
            if (dataGridStats->DataSource == nullptr) {
                MessageBox::Show("Нет данных для сохранения.", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            OdbcConnection^ conn = nullptr;
            try {
                conn = gcnew OdbcConnection(connStr);
                conn->Open();

                // Получаем текущий DataTable из dataGridStats
                DataTable^ currentStatsTable = safe_cast<DataTable^>(dataGridStats->DataSource);

                // Определяем тип статистики по наличию колонок
                bool isGoalkeeperStats = currentStatsTable->Columns->Contains("пропущенные мячи");
                bool isFieldPlayerStats = currentStatsTable->Columns->Contains("голы") && currentStatsTable->Columns->Contains("голевые передачи");

                if (isFieldPlayerStats) {
                    // Сохранение статистики полевых игроков
                    OdbcCommand^ insertCmd = gcnew OdbcCommand("INSERT INTO \"Statistika\" (id, \"матчи\", \"голы\", \"голевые передачи\") VALUES (?, ?, ?, ?);", conn);
                    insertCmd->Parameters->Add(gcnew OdbcParameter("id", OdbcType::Int));
                    insertCmd->Parameters->Add(gcnew OdbcParameter("матчи", OdbcType::Int));
                    insertCmd->Parameters->Add(gcnew OdbcParameter("голы", OdbcType::Int));
                    insertCmd->Parameters->Add(gcnew OdbcParameter("голевые передачи", OdbcType::Int));

                    OdbcCommand^ updateCmd = gcnew OdbcCommand("UPDATE \"Statistika\" SET \"матчи\"=?, \"голы\"=?, \"голевые передачи\"=? WHERE id=?;", conn);
                    updateCmd->Parameters->Add(gcnew OdbcParameter("матчи", OdbcType::Int));
                    updateCmd->Parameters->Add(gcnew OdbcParameter("голы", OdbcType::Int));
                    updateCmd->Parameters->Add(gcnew OdbcParameter("голевые передачи", OdbcType::Int));
                    updateCmd->Parameters->Add(gcnew OdbcParameter("id", OdbcType::Int));

                    OdbcCommand^ deleteCmd = gcnew OdbcCommand("DELETE FROM \"Statistika\" WHERE id=?;", conn);
                    deleteCmd->Parameters->Add(gcnew OdbcParameter("id", OdbcType::Int));

                    array<DataRow^>^ rows = gcnew array<DataRow^>(currentStatsTable->Rows->Count);
                    currentStatsTable->Rows->CopyTo(rows, 0);

                    for each (DataRow ^ row in rows) {
                        if (row->RowState == DataRowState::Added) {
                            int nextId = GetNextAvailableId("Statistika", conn);
                            insertCmd->Parameters[0]->Value = nextId;
                            insertCmd->Parameters[1]->Value = row->IsNull("матчи") ? 0 : row["матчи"];
                            insertCmd->Parameters[2]->Value = row->IsNull("голы") ? 0 : row["голы"];
                            insertCmd->Parameters[3]->Value = row->IsNull("голевые передачи") ? 0 : row["голевые передачи"];
                            insertCmd->ExecuteNonQuery();
                            row["id"] = nextId;
                            row->AcceptChanges();
                        }
                        else if (row->RowState == DataRowState::Modified) {
                            updateCmd->Parameters[0]->Value = row->IsNull("матчи") ? 0 : row["матчи"];
                            updateCmd->Parameters[1]->Value = row->IsNull("голы") ? 0 : row["голы"];
                            updateCmd->Parameters[2]->Value = row->IsNull("голевые передачи") ? 0 : row["голевые передачи"];
                            updateCmd->Parameters[3]->Value = row["id"];
                            updateCmd->ExecuteNonQuery();
                            row->AcceptChanges();
                        }
                    }

                    // Обработка удаленных строк
                    array<DataRow^>^ delRows = currentStatsTable->Select(nullptr, nullptr, DataViewRowState::Deleted);
                    for each (DataRow ^ drow in delRows) {
                        Object^ origId = drow["id", DataRowVersion::Original];
                        deleteCmd->Parameters[0]->Value = origId;
                        deleteCmd->ExecuteNonQuery();
                        drow->AcceptChanges();
                    }

                    MessageBox::Show("Статистика полевых игроков сохранена!", "Успех",
                        MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
                else if (isGoalkeeperStats) {
                    // Сохранение статистики вратарей
                    OdbcCommand^ insertCmd = gcnew OdbcCommand("INSERT INTO \"Statgp\" (id, \"матчи\", \"пропущенные мячи\") VALUES (?, ?, ?);", conn);
                    insertCmd->Parameters->Add(gcnew OdbcParameter("id", OdbcType::Int));
                    insertCmd->Parameters->Add(gcnew OdbcParameter("матчи", OdbcType::Int));
                    insertCmd->Parameters->Add(gcnew OdbcParameter("пропущенные мячи", OdbcType::Int));

                    OdbcCommand^ updateCmd = gcnew OdbcCommand("UPDATE \"Statgp\" SET \"матчи\"=?, \"пропущенные мячи\"=? WHERE id=?;", conn);
                    updateCmd->Parameters->Add(gcnew OdbcParameter("матчи", OdbcType::Int));
                    updateCmd->Parameters->Add(gcnew OdbcParameter("пропущенные мячи", OdbcType::Int));
                    updateCmd->Parameters->Add(gcnew OdbcParameter("id", OdbcType::Int));

                    OdbcCommand^ deleteCmd = gcnew OdbcCommand("DELETE FROM \"Statgp\" WHERE id=?;", conn);
                    deleteCmd->Parameters->Add(gcnew OdbcParameter("id", OdbcType::Int));

                    array<DataRow^>^ rows = gcnew array<DataRow^>(currentStatsTable->Rows->Count);
                    currentStatsTable->Rows->CopyTo(rows, 0);

                    for each (DataRow ^ row in rows) {
                        if (row->RowState == DataRowState::Added) {
                            int nextId = GetNextAvailableId("Statgp", conn);
                            insertCmd->Parameters[0]->Value = nextId;
                            insertCmd->Parameters[1]->Value = row->IsNull("матчи") ? 0 : row["матчи"];
                            insertCmd->Parameters[2]->Value = row->IsNull("пропущенные мячи") ? 0 : row["пропущенные мячи"];
                            insertCmd->ExecuteNonQuery();
                            row["id"] = nextId;
                            row->AcceptChanges();
                        }
                        else if (row->RowState == DataRowState::Modified) {
                            updateCmd->Parameters[0]->Value = row->IsNull("матчи") ? 0 : row["матчи"];
                            updateCmd->Parameters[1]->Value = row->IsNull("пропущенные мячи") ? 0 : row["пропущенные мячи"];
                            updateCmd->Parameters[2]->Value = row["id"];
                            updateCmd->ExecuteNonQuery();
                            row->AcceptChanges();
                        }
                    }

                    // Обработка удаленных строк
                    array<DataRow^>^ delRows = currentStatsTable->Select(nullptr, nullptr, DataViewRowState::Deleted);
                    for each (DataRow ^ drow in delRows) {
                        Object^ origId = drow["id", DataRowVersion::Original];
                        deleteCmd->Parameters[0]->Value = origId;
                        deleteCmd->ExecuteNonQuery();
                        drow->AcceptChanges();
                    }

                    MessageBox::Show("Статистика вратарей сохранена!", "Успех",
                        MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
                else {
                    // Если не можем определить тип, показываем информацию о колонках для отладки
                    String^ columnInfo = "Колонки в таблице: ";
                    for each (DataColumn ^ col in currentStatsTable->Columns) {
                        columnInfo += col->ColumnName + " ";
                    }
                    MessageBox::Show("Неизвестный формат статистики.\n" + columnInfo, "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Warning);
                }

                conn->Close();

                // Обновляем данные после сохранения
                if (isFieldPlayerStats) {
                    LoadStats();
                }
                else if (isGoalkeeperStats) {
                    LoadGoalkeepers();
                }

            }
            catch (Exception^ ex) {
                try { if (conn != nullptr) conn->Close(); }
                catch (...) {}
                MessageBox::Show("Ошибка сохранения статистики: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnSaveMatches_Click(System::Object^ sender, System::EventArgs^ e) {
            if (matchesTable == nullptr) {
                MessageBox::Show("Нет данных для сохранения матчей.", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            OdbcConnection^ conn = nullptr;
            try {
                conn = gcnew OdbcConnection(connStr);
                conn->Open();

                OdbcCommand^ insertCmd = gcnew OdbcCommand("INSERT INTO \"igri\" (id, date, hause, gast, goalhause, goalgast) VALUES (?, ?, ?, ?, ?, ?);", conn);
                insertCmd->Parameters->Add(gcnew OdbcParameter("id", OdbcType::Int, 0, "id"));
                insertCmd->Parameters->Add(gcnew OdbcParameter("date", OdbcType::Date, 0, "date"));
                insertCmd->Parameters->Add(gcnew OdbcParameter("hause", OdbcType::VarChar, 0, "hause"));
                insertCmd->Parameters->Add(gcnew OdbcParameter("gast", OdbcType::VarChar, 0, "gast"));
                insertCmd->Parameters->Add(gcnew OdbcParameter("goalhause", OdbcType::Int, 0, "goalhause"));
                insertCmd->Parameters->Add(gcnew OdbcParameter("goalgast", OdbcType::Int, 0, "goalgast"));

                array<DataRow^>^ rows = gcnew array<DataRow^>(matchesTable->Rows->Count);
                matchesTable->Rows->CopyTo(rows, 0);

                for each (DataRow ^ row in rows) {
                    if (row->RowState == DataRowState::Added) {
                        int nextId = GetNextAvailableId("igri", conn);
                        insertCmd->Parameters["id"]->Value = nextId;
                        insertCmd->Parameters["date"]->Value = row->IsNull("date") ? (Object^)DBNull::Value : row["date"];
                        insertCmd->Parameters["hause"]->Value = row->IsNull("hause") ? (Object^)DBNull::Value : row["hause"];
                        insertCmd->Parameters["gast"]->Value = row->IsNull("gast") ? (Object^)DBNull::Value : row["gast"];
                        insertCmd->Parameters["goalhause"]->Value = row->IsNull("goalhause") ? (Object^)DBNull::Value : row["goalhause"];
                        insertCmd->Parameters["goalgast"]->Value = row->IsNull("goalgast") ? (Object^)DBNull::Value : row["goalgast"];
                        insertCmd->ExecuteNonQuery();
                        row["id"] = nextId;
                        row->AcceptChanges();
                    }
                    else if (row->RowState == DataRowState::Modified) {
                        OdbcCommand^ updateCmd = gcnew OdbcCommand("UPDATE \"igri\" SET date=?, hause=?, gast=?, goalhause=?, goalgast=? WHERE id=?;", conn);
                        updateCmd->Parameters->AddWithValue("date", row->IsNull("date") ? (Object^)DBNull::Value : row["date"]);
                        updateCmd->Parameters->AddWithValue("hause", row->IsNull("hause") ? (Object^)DBNull::Value : row["hause"]);
                        updateCmd->Parameters->AddWithValue("gast", row->IsNull("gast") ? (Object^)DBNull::Value : row["gast"]);
                        updateCmd->Parameters->AddWithValue("goalhause", row->IsNull("goalhause") ? (Object^)DBNull::Value : row["goalhause"]);
                        updateCmd->Parameters->AddWithValue("goalgast", row->IsNull("goalgast") ? (Object^)DBNull::Value : row["goalgast"]);
                        updateCmd->Parameters->AddWithValue("id", row["id"]);
                        updateCmd->ExecuteNonQuery();
                        row->AcceptChanges();
                    }
                }

                conn->Close();
                MessageBox::Show("Матчи сохранены.", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                LoadMatches();
            }
            catch (Exception^ ex) {
                try { if (conn != nullptr) conn->Close(); }
                catch (...) {}
                MessageBox::Show("Ошибка сохранения матчей: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnSaveMatchEvents_Click(System::Object^ sender, System::EventArgs^ e) {
            if (matchEventsTable == nullptr) return;

            OdbcConnection^ conn = nullptr;
            try {
                conn = gcnew OdbcConnection(connStr);
                conn->Open();

                OdbcCommand^ insertCmd = gcnew OdbcCommand(
                    "INSERT INTO matchevents (match_id, player_name, event_type, minute, details) VALUES (?, ?, ?, ?, ?);", conn);
                insertCmd->Parameters->Add(gcnew OdbcParameter("match_id", OdbcType::Int));
                insertCmd->Parameters->Add(gcnew OdbcParameter("player_name", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("event_type", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("minute", OdbcType::Int));
                insertCmd->Parameters->Add(gcnew OdbcParameter("details", OdbcType::Text));

                OdbcCommand^ updateCmd = gcnew OdbcCommand(
                    "UPDATE matchevents SET match_id=?, player_name=?, event_type=?, minute=?, details=? WHERE event_id=?;", conn);
                updateCmd->Parameters->Add(gcnew OdbcParameter("match_id", OdbcType::Int));
                updateCmd->Parameters->Add(gcnew OdbcParameter("player_name", OdbcType::VarChar));
                updateCmd->Parameters->Add(gcnew OdbcParameter("event_type", OdbcType::VarChar));
                updateCmd->Parameters->Add(gcnew OdbcParameter("minute", OdbcType::Int));
                updateCmd->Parameters->Add(gcnew OdbcParameter("details", OdbcType::Text));
                updateCmd->Parameters->Add(gcnew OdbcParameter("event_id", OdbcType::Int));

                OdbcCommand^ deleteCmd = gcnew OdbcCommand("DELETE FROM matchevents WHERE event_id=?;", conn);
                deleteCmd->Parameters->Add(gcnew OdbcParameter("event_id", OdbcType::Int));

                array<DataRow^>^ rows = gcnew array<DataRow^>(matchEventsTable->Rows->Count);
                matchEventsTable->Rows->CopyTo(rows, 0);

                for each (DataRow ^ row in rows) {
                    if (row->RowState == DataRowState::Added) {
                        insertCmd->Parameters[0]->Value = row->IsNull("match_id") ? (Object^)DBNull::Value : row["match_id"];
                        insertCmd->Parameters[1]->Value = row->IsNull("Игрок") ? (Object^)DBNull::Value : row["Игрок"];
                        insertCmd->Parameters[2]->Value = row->IsNull("Событие") ? (Object^)DBNull::Value : row["Событие"];
                        insertCmd->Parameters[3]->Value = row->IsNull("Минута") ? (Object^)DBNull::Value : row["Минута"];
                        insertCmd->Parameters[4]->Value = row->IsNull("Детали") ? (Object^)DBNull::Value : row["Детали"];
                        insertCmd->ExecuteNonQuery();
                        row->AcceptChanges();
                    }
                    else if (row->RowState == DataRowState::Modified) {
                        updateCmd->Parameters[0]->Value = row->IsNull("match_id") ? (Object^)DBNull::Value : row["match_id"];
                        updateCmd->Parameters[1]->Value = row->IsNull("Игрок") ? (Object^)DBNull::Value : row["Игрок"];
                        updateCmd->Parameters[2]->Value = row->IsNull("Событие") ? (Object^)DBNull::Value : row["Событие"];
                        updateCmd->Parameters[3]->Value = row->IsNull("Минута") ? (Object^)DBNull::Value : row["Минута"];
                        updateCmd->Parameters[4]->Value = row->IsNull("Детали") ? (Object^)DBNull::Value : row["Детали"];
                        updateCmd->Parameters[5]->Value = row["event_id"];
                        updateCmd->ExecuteNonQuery();
                        row->AcceptChanges();
                    }
                }

                // Обработка удаленных строк
                array<DataRow^>^ delRows = matchEventsTable->Select(nullptr, nullptr, DataViewRowState::Deleted);
                for each (DataRow ^ drow in delRows) {
                    Object^ origId = drow["event_id", DataRowVersion::Original];
                    deleteCmd->Parameters[0]->Value = origId;
                    deleteCmd->ExecuteNonQuery();
                    drow->AcceptChanges();
                }

                conn->Close();
                MessageBox::Show("События матчей сохранены!", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                LoadMatchEvents();
            }
            catch (Exception^ ex) {
                try { if (conn != nullptr) conn->Close(); }
                catch (...) {}
                MessageBox::Show("Ошибка сохранения событий матчей: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnSaveLineups_Click(System::Object^ sender, System::EventArgs^ e) {
            if (lineupsTable == nullptr) return;

            OdbcConnection^ conn = nullptr;
            try {
                conn = gcnew OdbcConnection(connStr);
                conn->Open();

                OdbcCommand^ insertCmd = gcnew OdbcCommand(
                    "INSERT INTO matchlineups (match_id, player_name, position, is_starter) VALUES (?, ?, ?, ?);", conn);
                insertCmd->Parameters->Add(gcnew OdbcParameter("match_id", OdbcType::Int));
                insertCmd->Parameters->Add(gcnew OdbcParameter("player_name", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("position", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("is_starter", OdbcType::Bit));

                OdbcCommand^ updateCmd = gcnew OdbcCommand(
                    "UPDATE matchlineups SET match_id=?, player_name=?, position=?, is_starter=? WHERE lineup_id=?;", conn);
                updateCmd->Parameters->Add(gcnew OdbcParameter("match_id", OdbcType::Int));
                updateCmd->Parameters->Add(gcnew OdbcParameter("player_name", OdbcType::VarChar));
                updateCmd->Parameters->Add(gcnew OdbcParameter("position", OdbcType::VarChar));
                updateCmd->Parameters->Add(gcnew OdbcParameter("is_starter", OdbcType::Bit));
                updateCmd->Parameters->Add(gcnew OdbcParameter("lineup_id", OdbcType::Int));

                OdbcCommand^ deleteCmd = gcnew OdbcCommand("DELETE FROM matchlineups WHERE lineup_id=?;", conn);
                deleteCmd->Parameters->Add(gcnew OdbcParameter("lineup_id", OdbcType::Int));

                array<DataRow^>^ rows = gcnew array<DataRow^>(lineupsTable->Rows->Count);
                lineupsTable->Rows->CopyTo(rows, 0);

                for each (DataRow ^ row in rows) {
                    bool isStarter = false;
                    if (!row->IsNull("Статус")) {
                        isStarter = (row["Статус"]->ToString() == "Основной");
                    }

                    if (row->RowState == DataRowState::Added) {
                        insertCmd->Parameters[0]->Value = row->IsNull("match_id") ? (Object^)DBNull::Value : row["match_id"];
                        insertCmd->Parameters[1]->Value = row->IsNull("Игрок") ? (Object^)DBNull::Value : row["Игрок"];
                        insertCmd->Parameters[2]->Value = row->IsNull("Позиция") ? (Object^)DBNull::Value : row["Позиция"];
                        insertCmd->Parameters[3]->Value = isStarter;
                        insertCmd->ExecuteNonQuery();
                        row->AcceptChanges();
                    }
                    else if (row->RowState == DataRowState::Modified) {
                        updateCmd->Parameters[0]->Value = row->IsNull("match_id") ? (Object^)DBNull::Value : row["match_id"];
                        updateCmd->Parameters[1]->Value = row->IsNull("Игрок") ? (Object^)DBNull::Value : row["Игрок"];
                        updateCmd->Parameters[2]->Value = row->IsNull("Позиция") ? (Object^)DBNull::Value : row["Позиция"];
                        updateCmd->Parameters[3]->Value = isStarter;
                        updateCmd->Parameters[4]->Value = row["lineup_id"];
                        updateCmd->ExecuteNonQuery();
                        row->AcceptChanges();
                    }
                }

                // Обработка удаленных строк
                array<DataRow^>^ delRows = lineupsTable->Select(nullptr, nullptr, DataViewRowState::Deleted);
                for each (DataRow ^ drow in delRows) {
                    Object^ origId = drow["lineup_id", DataRowVersion::Original];
                    deleteCmd->Parameters[0]->Value = origId;
                    deleteCmd->ExecuteNonQuery();
                    drow->AcceptChanges();
                }

                conn->Close();
                MessageBox::Show("Составы сохранены!", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                LoadLineups();
            }
            catch (Exception^ ex) {
                try { if (conn != nullptr) conn->Close(); }
                catch (...) {}
                MessageBox::Show("Ошибка сохранения составов: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnSaveInjuries_Click(System::Object^ sender, System::EventArgs^ e) {
            if (injuriesTable == nullptr) return;

            OdbcConnection^ conn = nullptr;
            try {
                conn = gcnew OdbcConnection(connStr);
                conn->Open();

                OdbcCommand^ insertCmd = gcnew OdbcCommand(
                    "INSERT INTO injuries (player_name, injury_type, date_start, date_end, status) VALUES (?, ?, ?, ?, ?);", conn);
                insertCmd->Parameters->Add(gcnew OdbcParameter("player_name", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("injury_type", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("date_start", OdbcType::Date));
                insertCmd->Parameters->Add(gcnew OdbcParameter("date_end", OdbcType::Date));
                insertCmd->Parameters->Add(gcnew OdbcParameter("status", OdbcType::VarChar));

                OdbcCommand^ updateCmd = gcnew OdbcCommand(
                    "UPDATE injuries SET player_name=?, injury_type=?, date_start=?, date_end=?, status=? WHERE injury_id=?;", conn);
                updateCmd->Parameters->Add(gcnew OdbcParameter("player_name", OdbcType::VarChar));
                updateCmd->Parameters->Add(gcnew OdbcParameter("injury_type", OdbcType::VarChar));
                updateCmd->Parameters->Add(gcnew OdbcParameter("date_start", OdbcType::Date));
                updateCmd->Parameters->Add(gcnew OdbcParameter("date_end", OdbcType::Date));
                updateCmd->Parameters->Add(gcnew OdbcParameter("status", OdbcType::VarChar));
                updateCmd->Parameters->Add(gcnew OdbcParameter("injury_id", OdbcType::Int));

                OdbcCommand^ deleteCmd = gcnew OdbcCommand("DELETE FROM injuries WHERE injury_id=?;", conn);
                deleteCmd->Parameters->Add(gcnew OdbcParameter("injury_id", OdbcType::Int));

                array<DataRow^>^ rows = gcnew array<DataRow^>(injuriesTable->Rows->Count);
                injuriesTable->Rows->CopyTo(rows, 0);

                for each (DataRow ^ row in rows) {
                    if (row->RowState == DataRowState::Added) {
                        insertCmd->Parameters[0]->Value = row->IsNull("Игрок") ? (Object^)DBNull::Value : row["Игрок"];
                        insertCmd->Parameters[1]->Value = row->IsNull("Тип травмы") ? (Object^)DBNull::Value : row["Тип травмы"];
                        insertCmd->Parameters[2]->Value = row->IsNull("Начало") ? (Object^)DBNull::Value : row["Начало"];
                        insertCmd->Parameters[3]->Value = row->IsNull("Конец") ? (Object^)DBNull::Value : row["Конец"];
                        insertCmd->Parameters[4]->Value = row->IsNull("Статус") ? (Object^)DBNull::Value : row["Статус"];
                        insertCmd->ExecuteNonQuery();
                        row->AcceptChanges();
                    }
                    else if (row->RowState == DataRowState::Modified) {
                        updateCmd->Parameters[0]->Value = row->IsNull("Игрок") ? (Object^)DBNull::Value : row["Игрок"];
                        updateCmd->Parameters[1]->Value = row->IsNull("Тип травмы") ? (Object^)DBNull::Value : row["Тип травмы"];
                        updateCmd->Parameters[2]->Value = row->IsNull("Начало") ? (Object^)DBNull::Value : row["Начало"];
                        updateCmd->Parameters[3]->Value = row->IsNull("Конец") ? (Object^)DBNull::Value : row["Конец"];
                        updateCmd->Parameters[4]->Value = row->IsNull("Статус") ? (Object^)DBNull::Value : row["Статус"];
                        updateCmd->Parameters[5]->Value = row["injury_id"];
                        updateCmd->ExecuteNonQuery();
                        row->AcceptChanges();
                    }
                }

                // Обработка удаленных строк
                array<DataRow^>^ delRows = injuriesTable->Select(nullptr, nullptr, DataViewRowState::Deleted);
                for each (DataRow ^ drow in delRows) {
                    Object^ origId = drow["injury_id", DataRowVersion::Original];
                    deleteCmd->Parameters[0]->Value = origId;
                    deleteCmd->ExecuteNonQuery();
                    drow->AcceptChanges();
                }

                conn->Close();
                MessageBox::Show("Травмы сохранены!", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                LoadInjuries();
            }
            catch (Exception^ ex) {
                try { if (conn != nullptr) conn->Close(); }
                catch (...) {}
                MessageBox::Show("Ошибка сохранения травм: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnSaveTrainings_Click(System::Object^ sender, System::EventArgs^ e) {
            if (trainingsTable == nullptr) return;

            OdbcConnection^ conn = nullptr;
            try {
                conn = gcnew OdbcConnection(connStr);
                conn->Open();

                OdbcCommand^ insertCmd = gcnew OdbcCommand(
                    "INSERT INTO trainings (training_date, focus_area, participants) VALUES (?, ?, ?);", conn);
                insertCmd->Parameters->Add(gcnew OdbcParameter("training_date", OdbcType::Date));
                insertCmd->Parameters->Add(gcnew OdbcParameter("focus_area", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("participants", OdbcType::Text));

                OdbcCommand^ updateCmd = gcnew OdbcCommand(
                    "UPDATE trainings SET training_date=?, focus_area=?, participants=? WHERE training_id=?;", conn);
                updateCmd->Parameters->Add(gcnew OdbcParameter("training_date", OdbcType::Date));
                updateCmd->Parameters->Add(gcnew OdbcParameter("focus_area", OdbcType::VarChar));
                updateCmd->Parameters->Add(gcnew OdbcParameter("participants", OdbcType::Text));
                updateCmd->Parameters->Add(gcnew OdbcParameter("training_id", OdbcType::Int));

                OdbcCommand^ deleteCmd = gcnew OdbcCommand("DELETE FROM trainings WHERE training_id=?;", conn);
                deleteCmd->Parameters->Add(gcnew OdbcParameter("training_id", OdbcType::Int));

                array<DataRow^>^ rows = gcnew array<DataRow^>(trainingsTable->Rows->Count);
                trainingsTable->Rows->CopyTo(rows, 0);

                for each (DataRow ^ row in rows) {
                    if (row->RowState == DataRowState::Added) {
                        insertCmd->Parameters[0]->Value = row->IsNull("Дата") ? (Object^)DBNull::Value : row["Дата"];
                        insertCmd->Parameters[1]->Value = row->IsNull("Фокус тренировки") ? (Object^)DBNull::Value : row["Фокус тренировки"];
                        insertCmd->Parameters[2]->Value = row->IsNull("Участники") ? (Object^)DBNull::Value : row["Участники"];
                        insertCmd->ExecuteNonQuery();
                        row->AcceptChanges();
                    }
                    else if (row->RowState == DataRowState::Modified) {
                        updateCmd->Parameters[0]->Value = row->IsNull("Дата") ? (Object^)DBNull::Value : row["Дата"];
                        updateCmd->Parameters[1]->Value = row->IsNull("Фокус тренировки") ? (Object^)DBNull::Value : row["Фокус тренировки"];
                        updateCmd->Parameters[2]->Value = row->IsNull("Участники") ? (Object^)DBNull::Value : row["Участники"];
                        updateCmd->Parameters[3]->Value = row["training_id"];
                        updateCmd->ExecuteNonQuery();
                        row->AcceptChanges();
                    }
                }

                // Обработка удаленных строк
                array<DataRow^>^ delRows = trainingsTable->Select(nullptr, nullptr, DataViewRowState::Deleted);
                for each (DataRow ^ drow in delRows) {
                    Object^ origId = drow["training_id", DataRowVersion::Original];
                    deleteCmd->Parameters[0]->Value = origId;
                    deleteCmd->ExecuteNonQuery();
                    drow->AcceptChanges();
                }

                conn->Close();
                MessageBox::Show("Тренировки сохранены!", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                LoadTrainings();
            }
            catch (Exception^ ex) {
                try { if (conn != nullptr) conn->Close(); }
                catch (...) {}
                MessageBox::Show("Ошибка сохранения тренировок: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        // МЕТОДЫ УДАЛЕНИЯ ДАННЫХ
        System::Void btnDeletePlayer_Click(System::Object^ sender, System::EventArgs^ e) {
            if (dataGridPlayers->CurrentRow == nullptr) {
                MessageBox::Show("Выберите игрока для удаления!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            DataRowView^ drv = dynamic_cast<DataRowView^>(dataGridPlayers->CurrentRow->DataBoundItem);
            if (drv == nullptr) return;
            DataRow^ row = drv->Row;
            if (row == nullptr) return;

            System::Windows::Forms::DialogResult result = MessageBox::Show("Вы уверены, что хотите удалить этого игрока?",
                "Подтверждение удаления", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

            if (result != System::Windows::Forms::DialogResult::Yes) return;

            try {
                int playerId = Convert::ToInt32(row["id"]);

                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                conn->Open();

                OdbcCommand^ cmd1 = gcnew OdbcCommand("DELETE FROM \"Statistika\" WHERE id = ?;", conn);
                cmd1->Parameters->AddWithValue("id", playerId);
                cmd1->ExecuteNonQuery();

                OdbcCommand^ cmd2 = gcnew OdbcCommand("DELETE FROM \"Statgp\" WHERE id = ?;", conn);
                cmd2->Parameters->AddWithValue("id", playerId);
                cmd2->ExecuteNonQuery();

                OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM \"Player\" WHERE id = ?;", conn);
                cmd->Parameters->AddWithValue("id", playerId);
                cmd->ExecuteNonQuery();

                conn->Close();

                LoadPlayers();
                LoadStats();
                MessageBox::Show("Игрок успешно удалён.", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка при удалении игрока: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnDeleteMatch_Click(System::Object^ sender, System::EventArgs^ e) {
            DataGridViewRow^ row = nullptr;
            DataRowView^ drv = nullptr;
            if (dataGridStats->DataSource == matchesTable && dataGridStats->CurrentRow != nullptr) {
                drv = dynamic_cast<DataRowView^>(dataGridStats->CurrentRow->DataBoundItem);
            }
            else if (dataGridMatches->DataSource == matchesTable && dataGridMatches->CurrentRow != nullptr) {
                drv = dynamic_cast<DataRowView^>(dataGridMatches->CurrentRow->DataBoundItem);
            }

            if (drv == nullptr) {
                MessageBox::Show("Выберите матч для удаления!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            DataRow^ dr = drv->Row;
            if (dr == nullptr) return;

            System::Windows::Forms::DialogResult result = MessageBox::Show("Удалить выбранный матч?",
                "Подтверждение удаления", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

            if (result != System::Windows::Forms::DialogResult::Yes) return;

            try {
                int matchId = Convert::ToInt32(dr["id"]);
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                conn->Open();
                OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM \"igri\" WHERE id = ?;", conn);
                cmd->Parameters->AddWithValue("id", matchId);
                cmd->ExecuteNonQuery();
                conn->Close();

                LoadMatches();
                MessageBox::Show("Матч удалён.", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка удаления матча: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnDeleteMatchEvent_Click(System::Object^ sender, System::EventArgs^ e) {
            if (dataGridMatchEvents->CurrentRow == nullptr || dataGridMatchEvents->CurrentRow->IsNewRow) {
                MessageBox::Show("Выберите событие для удаления!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            DataRowView^ drv = dynamic_cast<DataRowView^>(dataGridMatchEvents->CurrentRow->DataBoundItem);
            if (drv == nullptr) return;

            System::Windows::Forms::DialogResult result = MessageBox::Show("Удалить выбранное событие?",
                "Подтверждение удаления", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

            if (result != System::Windows::Forms::DialogResult::Yes) return;

            try {
                int eventId = Convert::ToInt32(drv->Row["event_id"]);
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                conn->Open();
                OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM matchevents WHERE event_id = ?;", conn);
                cmd->Parameters->AddWithValue("event_id", eventId);
                cmd->ExecuteNonQuery();
                conn->Close();

                LoadMatchEvents();
                MessageBox::Show("Событие удалено.", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка удаления события: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnDeleteLineup_Click(System::Object^ sender, System::EventArgs^ e) {
            if (dataGridLineups->CurrentRow == nullptr || dataGridLineups->CurrentRow->IsNewRow) {
                MessageBox::Show("Выберите состав для удаления!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            DataRowView^ drv = dynamic_cast<DataRowView^>(dataGridLineups->CurrentRow->DataBoundItem);
            if (drv == nullptr) return;

            System::Windows::Forms::DialogResult result = MessageBox::Show("Удалить выбранный состав?",
                "Подтверждение удаления", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

            if (result != System::Windows::Forms::DialogResult::Yes) return;

            try {
                int lineupId = Convert::ToInt32(drv->Row["lineup_id"]);
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                conn->Open();
                OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM matchlineups WHERE lineup_id = ?;", conn);
                cmd->Parameters->AddWithValue("lineup_id", lineupId);
                cmd->ExecuteNonQuery();
                conn->Close();

                LoadLineups();
                MessageBox::Show("Состав удален.", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка удаления состава: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnDeleteInjury_Click(System::Object^ sender, System::EventArgs^ e) {
            if (dataGridInjuries->CurrentRow == nullptr || dataGridInjuries->CurrentRow->IsNewRow) {
                MessageBox::Show("Выберите травму для удаления!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            DataRowView^ drv = dynamic_cast<DataRowView^>(dataGridInjuries->CurrentRow->DataBoundItem);
            if (drv == nullptr) return;

            System::Windows::Forms::DialogResult result = MessageBox::Show("Удалить выбранную травму?",
                "Подтверждение удаления", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

            if (result != System::Windows::Forms::DialogResult::Yes) return;

            try {
                int injuryId = Convert::ToInt32(drv->Row["injury_id"]);
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                conn->Open();
                OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM injuries WHERE injury_id = ?;", conn);
                cmd->Parameters->AddWithValue("injury_id", injuryId);
                cmd->ExecuteNonQuery();
                conn->Close();

                LoadInjuries();
                MessageBox::Show("Травма удалена.", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка удаления травмы: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void btnDeleteTraining_Click(System::Object^ sender, System::EventArgs^ e) {
            if (dataGridTrainings->CurrentRow == nullptr || dataGridTrainings->CurrentRow->IsNewRow) {
                MessageBox::Show("Выберите тренировку для удаления!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            DataRowView^ drv = dynamic_cast<DataRowView^>(dataGridTrainings->CurrentRow->DataBoundItem);
            if (drv == nullptr) return;

            System::Windows::Forms::DialogResult result = MessageBox::Show("Удалить выбранную тренировку?",
                "Подтверждение удаления", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

            if (result != System::Windows::Forms::DialogResult::Yes) return;

            try {
                int trainingId = Convert::ToInt32(drv->Row["training_id"]);
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                conn->Open();
                OdbcCommand^ cmd = gcnew OdbcCommand("DELETE FROM trainings WHERE training_id = ?;", conn);
                cmd->Parameters->AddWithValue("training_id", trainingId);
                cmd->ExecuteNonQuery();
                conn->Close();

                LoadTrainings();
                MessageBox::Show("Тренировка удалена.", "Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка удаления тренировки: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        // Методы меню (заглушки)
        System::Void ExportData(System::Object^ sender, System::EventArgs^ e) {
            MessageBox::Show("Экспорт данных в разработке", "Информация",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        System::Void ImportData(System::Object^ sender, System::EventArgs^ e) {
            MessageBox::Show("Импорт данных в разработке", "Информация",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        System::Void ExitApp(System::Object^ sender, System::EventArgs^ e) {
            this->Close();
        }

        System::Void SaveAllData(System::Object^ sender, System::EventArgs^ e) {
            MessageBox::Show("Сохранение всех данных в разработке", "Информация",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        System::Void LoadAllData(System::Object^ sender, System::EventArgs^ e) {
            MessageBox::Show("Загрузка всех данных в разработке", "Информация",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        System::Void ClearCache(System::Object^ sender, System::EventArgs^ e) {
            playersCache = nullptr;
            matchesCache = nullptr;
            MessageBox::Show("Кэш очищен", "Информация",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        System::Void ShowTeamStats(System::Object^ sender, System::EventArgs^ e) {
            MessageBox::Show("Статистика команды в разработке", "Информация",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        System::Void ShowEfficiencyAnalysis(System::Object^ sender, System::EventArgs^ e) {
            MessageBox::Show("Анализ эффективности в разработке", "Информация",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        System::Void ShowInjuryReport(System::Object^ sender, System::EventArgs^ e) {
            MessageBox::Show("Отчет по травмам в разработке", "Информация",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        // Метод для установки ссылки на главную форму
        void SetMainForm(Form^ mainForm) {
            MyForm = mainForm;
        }
    };
}