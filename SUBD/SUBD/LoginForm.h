#pragma once
#include "AdminPanel.h"
#include <msclr\marshal_cppstd.h>
#include <Windows.h>
#include <sstream>
#include <iomanip>

namespace SUBD {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Data::Odbc;
    using namespace System::Drawing;
    using namespace System::Drawing::Drawing2D;
    using namespace System::Security::Cryptography;
    using namespace System::Text;

    public ref class LoginForm : public System::Windows::Forms::Form
    {
    public:
        LoginForm(void)
        {
            InitializeComponent();
            ApplyAwesomeDesign();
            InitializeLoginAttempts();
        }

    protected:
        ~LoginForm()
        {
            if (components) delete components;
        }

    private:
        System::Windows::Forms::TextBox^ txtLogin;
        System::Windows::Forms::TextBox^ txtPassword;
        System::Windows::Forms::Button^ btnLogin;
        System::Windows::Forms::Label^ lblLogin;
        System::Windows::Forms::Label^ lblPassword;
        System::Windows::Forms::Panel^ panelMain;
        System::Windows::Forms::Panel^ panelHeader;
        System::Windows::Forms::Label^ labelTitle;
        System::Windows::Forms::Label^ labelSubtitle;
        System::Windows::Forms::PictureBox^ pictureBox1;
        System::Windows::Forms::Panel^ panelLogin;
        System::Windows::Forms::Panel^ panelPassword;
        System::Windows::Forms::Button^ btnClose;
        System::Windows::Forms::Button^ btnMinimize;
        System::Windows::Forms::Label^ labelFooter;

        // НОВЫЕ ЭЛЕМЕНТЫ ДЛЯ БЛОКИРОВКИ
        System::Windows::Forms::Label^ lblAttempts;
        System::Windows::Forms::Label^ lblLockTimer;
        System::Windows::Forms::Timer^ lockTimer;

    private:
        System::ComponentModel::Container^ components;

        // НОВЫЕ ПЕРЕМЕННЫЕ ДЛЯ БЛОКИРОВКИ
        int failedAttempts;
        DateTime lockUntil;
        bool isAccountLocked;
        String^ connStr = "DSN=AliansDB;UID=postgres;PWD=Azard18031;";

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->txtLogin = (gcnew System::Windows::Forms::TextBox());
            this->txtPassword = (gcnew System::Windows::Forms::TextBox());
            this->btnLogin = (gcnew System::Windows::Forms::Button());
            this->lblLogin = (gcnew System::Windows::Forms::Label());
            this->lblPassword = (gcnew System::Windows::Forms::Label());
            this->panelMain = (gcnew System::Windows::Forms::Panel());
            this->lblLockTimer = (gcnew System::Windows::Forms::Label());  // НОВЫЙ
            this->lblAttempts = (gcnew System::Windows::Forms::Label());   // НОВЫЙ
            this->labelFooter = (gcnew System::Windows::Forms::Label());
            this->panelPassword = (gcnew System::Windows::Forms::Panel());
            this->panelLogin = (gcnew System::Windows::Forms::Panel());
            this->labelSubtitle = (gcnew System::Windows::Forms::Label());
            this->labelTitle = (gcnew System::Windows::Forms::Label());
            this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
            this->panelHeader = (gcnew System::Windows::Forms::Panel());
            this->btnMinimize = (gcnew System::Windows::Forms::Button());
            this->btnClose = (gcnew System::Windows::Forms::Button());
            this->lockTimer = (gcnew System::Windows::Forms::Timer(this->components));  // НОВЫЙ

            this->panelMain->SuspendLayout();
            this->panelPassword->SuspendLayout();
            this->panelLogin->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
            this->panelHeader->SuspendLayout();
            this->SuspendLayout();

            // txtLogin (остается без изменений)
            this->txtLogin->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(30)),
                static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(30)));
            this->txtLogin->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->txtLogin->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11, System::Drawing::FontStyle::Regular));
            this->txtLogin->ForeColor = System::Drawing::Color::White;
            this->txtLogin->Location = System::Drawing::Point(15, 10);
            this->txtLogin->Name = L"txtLogin";
            this->txtLogin->Size = System::Drawing::Size(270, 25);
            this->txtLogin->TabIndex = 0;
            this->txtLogin->Text = L"Admin";

            // txtPassword (остается без изменений)
            this->txtPassword->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(30)),
                static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(30)));
            this->txtPassword->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->txtPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11, System::Drawing::FontStyle::Regular));
            this->txtPassword->ForeColor = System::Drawing::Color::White;
            this->txtPassword->Location = System::Drawing::Point(15, 10);
            this->txtPassword->Name = L"txtPassword";
            this->txtPassword->PasswordChar = '●';
            this->txtPassword->Size = System::Drawing::Size(270, 25);
            this->txtPassword->TabIndex = 1;

            // btnLogin
            this->btnLogin->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)),
                static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
            this->btnLogin->FlatAppearance->BorderSize = 0;
            this->btnLogin->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnLogin->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11, System::Drawing::FontStyle::Bold));
            this->btnLogin->ForeColor = System::Drawing::Color::White;
            this->btnLogin->Location = System::Drawing::Point(50, 330);  // Изменено Y для новых элементов
            this->btnLogin->Name = L"btnLogin";
            this->btnLogin->Size = System::Drawing::Size(300, 45);
            this->btnLogin->TabIndex = 2;
            this->btnLogin->Text = L"🚀 ПРОДОЛЖИТЬ";
            this->btnLogin->UseVisualStyleBackColor = false;
            this->btnLogin->Click += gcnew System::EventHandler(this, &LoginForm::btnLogin_Click);

            // lblLogin
            this->lblLogin->AutoSize = true;
            this->lblLogin->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
            this->lblLogin->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(150)),
                static_cast<System::Int32>(static_cast<System::Byte>(150)), static_cast<System::Int32>(static_cast<System::Byte>(150)));
            this->lblLogin->Location = System::Drawing::Point(50, 150);
            this->lblLogin->Name = L"lblLogin";
            this->lblLogin->Size = System::Drawing::Size(55, 20);
            this->lblLogin->TabIndex = 3;
            this->lblLogin->Text = L"Логин:";

            // lblPassword
            this->lblPassword->AutoSize = true;
            this->lblPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
            this->lblPassword->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(150)),
                static_cast<System::Int32>(static_cast<System::Byte>(150)), static_cast<System::Int32>(static_cast<System::Byte>(150)));
            this->lblPassword->Location = System::Drawing::Point(50, 220);
            this->lblPassword->Name = L"lblPassword";
            this->lblPassword->Size = System::Drawing::Size(65, 20);
            this->lblPassword->TabIndex = 4;
            this->lblPassword->Text = L"Пароль:";

            // panelMain
            this->panelMain->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(25)),
                static_cast<System::Int32>(static_cast<System::Byte>(25)), static_cast<System::Int32>(static_cast<System::Byte>(25)));
            this->panelMain->Controls->Add(this->lblLockTimer);  // НОВЫЙ
            this->panelMain->Controls->Add(this->lblAttempts);   // НОВЫЙ
            this->panelMain->Controls->Add(this->labelFooter);
            this->panelMain->Controls->Add(this->panelPassword);
            this->panelMain->Controls->Add(this->panelLogin);
            this->panelMain->Controls->Add(this->labelSubtitle);
            this->panelMain->Controls->Add(this->labelTitle);
            this->panelMain->Controls->Add(this->pictureBox1);
            this->panelMain->Controls->Add(this->lblPassword);
            this->panelMain->Controls->Add(this->lblLogin);
            this->panelMain->Controls->Add(this->btnLogin);
            this->panelMain->Dock = System::Windows::Forms::DockStyle::Fill;
            this->panelMain->Location = System::Drawing::Point(0, 40);
            this->panelMain->Name = L"panelMain";
            this->panelMain->Size = System::Drawing::Size(400, 440);  // Увеличен размер
            this->panelMain->TabIndex = 5;

            // lblLockTimer (НОВЫЙ - таймер блокировки)
            this->lblLockTimer->AutoSize = true;
            this->lblLockTimer->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
            this->lblLockTimer->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(231)),
                static_cast<System::Int32>(static_cast<System::Byte>(76)), static_cast<System::Int32>(static_cast<System::Byte>(60)));
            this->lblLockTimer->Location = System::Drawing::Point(50, 300);
            this->lblLockTimer->Name = L"lblLockTimer";
            this->lblLockTimer->Size = System::Drawing::Size(0, 20);
            this->lblLockTimer->TabIndex = 12;
            this->lblLockTimer->Visible = false;

            // lblAttempts (НОВЫЙ - счетчик попыток)
            this->lblAttempts->AutoSize = true;
            this->lblAttempts->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
            this->lblAttempts->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(241)),
                static_cast<System::Int32>(static_cast<System::Byte>(196)), static_cast<System::Int32>(static_cast<System::Byte>(15)));
            this->lblAttempts->Location = System::Drawing::Point(50, 280);
            this->lblAttempts->Name = L"lblAttempts";
            this->lblAttempts->Size = System::Drawing::Size(0, 20);
            this->lblAttempts->TabIndex = 11;
            this->lblAttempts->Visible = false;

            // labelFooter
            this->labelFooter->AutoSize = true;
            this->labelFooter->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8));
            this->labelFooter->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)),
                static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)));
            this->labelFooter->Location = System::Drawing::Point(120, 410);
            this->labelFooter->Name = L"labelFooter";
            this->labelFooter->Size = System::Drawing::Size(169, 19);
            this->labelFooter->TabIndex = 10;
            this->labelFooter->Text = L"Football Manager © 2025";

            // panelPassword
            this->panelPassword->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)),
                static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(48)));
            this->panelPassword->Controls->Add(this->txtPassword);
            this->panelPassword->Location = System::Drawing::Point(50, 240);
            this->panelPassword->Name = L"panelPassword";
            this->panelPassword->Size = System::Drawing::Size(300, 35);
            this->panelPassword->TabIndex = 9;

            // panelLogin
            this->panelLogin->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)),
                static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(48)));
            this->panelLogin->Controls->Add(this->txtLogin);
            this->panelLogin->Location = System::Drawing::Point(50, 170);
            this->panelLogin->Name = L"panelLogin";
            this->panelLogin->Size = System::Drawing::Size(300, 35);
            this->panelLogin->TabIndex = 8;

            // labelSubtitle
            this->labelSubtitle->AutoSize = true;
            this->labelSubtitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
            this->labelSubtitle->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(150)),
                static_cast<System::Int32>(static_cast<System::Byte>(150)), static_cast<System::Int32>(static_cast<System::Byte>(150)));
            this->labelSubtitle->Location = System::Drawing::Point(47, 85);
            this->labelSubtitle->Name = L"labelSubtitle";
            this->labelSubtitle->Size = System::Drawing::Size(335, 20);
            this->labelSubtitle->TabIndex = 7;
            this->labelSubtitle->Text = L"Введите учетные данные для доступа к панели";

            // labelTitle
            this->labelTitle->AutoSize = true;
            this->labelTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 16, System::Drawing::FontStyle::Bold));
            this->labelTitle->ForeColor = System::Drawing::Color::White;
            this->labelTitle->Location = System::Drawing::Point(45, 50);
            this->labelTitle->Name = L"labelTitle";
            this->labelTitle->Size = System::Drawing::Size(264, 37);
            this->labelTitle->TabIndex = 6;
            this->labelTitle->Text = L"🔐 АВТОРИЗАЦИЯ";

            // pictureBox1
            this->pictureBox1->Location = System::Drawing::Point(320, 50);
            this->pictureBox1->Name = L"pictureBox1";
            this->pictureBox1->Size = System::Drawing::Size(40, 40);
            this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
            this->pictureBox1->TabIndex = 5;
            this->pictureBox1->TabStop = false;

            // panelHeader
            this->panelHeader->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)),
                static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
            this->panelHeader->Controls->Add(this->btnMinimize);
            this->panelHeader->Controls->Add(this->btnClose);
            this->panelHeader->Dock = System::Windows::Forms::DockStyle::Top;
            this->panelHeader->Location = System::Drawing::Point(0, 0);
            this->panelHeader->Name = L"panelHeader";
            this->panelHeader->Size = System::Drawing::Size(400, 40);
            this->panelHeader->TabIndex = 6;
            this->panelHeader->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &LoginForm::panelHeader_MouseDown);
            this->panelHeader->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &LoginForm::panelHeader_MouseMove);
            this->panelHeader->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &LoginForm::panelHeader_MouseUp);

            // btnMinimize
            this->btnMinimize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->btnMinimize->FlatAppearance->BorderSize = 0;
            this->btnMinimize->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)),
                static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(40)));
            this->btnMinimize->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnMinimize->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->btnMinimize->ForeColor = System::Drawing::Color::White;
            this->btnMinimize->Location = System::Drawing::Point(320, 0);
            this->btnMinimize->Name = L"btnMinimize";
            this->btnMinimize->Size = System::Drawing::Size(40, 40);
            this->btnMinimize->TabIndex = 1;
            this->btnMinimize->Text = L"─";
            this->btnMinimize->UseVisualStyleBackColor = true;
            this->btnMinimize->Click += gcnew System::EventHandler(this, &LoginForm::btnMinimize_Click);

            // btnClose
            this->btnClose->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->btnClose->FlatAppearance->BorderSize = 0;
            this->btnClose->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)),
                static_cast<System::Int32>(static_cast<System::Byte>(17)), static_cast<System::Int32>(static_cast<System::Byte>(35)));
            this->btnClose->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnClose->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
            this->btnClose->ForeColor = System::Drawing::Color::White;
            this->btnClose->Location = System::Drawing::Point(360, 0);
            this->btnClose->Name = L"btnClose";
            this->btnClose->Size = System::Drawing::Size(40, 40);
            this->btnClose->TabIndex = 0;
            this->btnClose->Text = L"✕";
            this->btnClose->UseVisualStyleBackColor = true;
            this->btnClose->Click += gcnew System::EventHandler(this, &LoginForm::btnClose_Click);

            // lockTimer (НОВЫЙ - таймер для отсчета блокировки)
            this->lockTimer->Interval = 1000;
            this->lockTimer->Tick += gcnew System::EventHandler(this, &LoginForm::lockTimer_Tick);

            // LoginForm
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
            this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)),
                static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(204)));
            this->ClientSize = System::Drawing::Size(400, 480);  // Увеличен размер
            this->Controls->Add(this->panelMain);
            this->Controls->Add(this->panelHeader);
            this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
            this->ForeColor = System::Drawing::Color::White;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->Name = L"LoginForm";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"Авторизация";

            this->panelMain->ResumeLayout(false);
            this->panelMain->PerformLayout();
            this->panelPassword->ResumeLayout(false);
            this->panelPassword->PerformLayout();
            this->panelLogin->ResumeLayout(false);
            this->panelLogin->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
            this->panelHeader->ResumeLayout(false);
            this->ResumeLayout(false);
        }
#pragma endregion

    private:
        bool dragging;
        Point dragCursorPoint;
        Point dragFormPoint;

        // ИСПРАВЛЕННЫЙ МЕТОД: Вычисление SHA256 хэша на чистом .NET
        String^ ComputeSHA256Hash(String^ input)
        {
            try
            {
                // Создаем объект SHA256
                SHA256^ sha256 = SHA256::Create();

                // Конвертируем строку в байты
                array<Byte>^ inputBytes = Encoding::UTF8->GetBytes(input);

                // Вычисляем хэш
                array<Byte>^ hashBytes = sha256->ComputeHash(inputBytes);

                // Конвертируем байты в hex строку
                StringBuilder^ sb = gcnew StringBuilder();
                for each (Byte b in hashBytes)
                {
                    sb->Append(b.ToString("x2"));
                }

                return sb->ToString();
            }
            catch (Exception^ ex)
            {
                // Альтернативный способ вывода
                MessageBox::Show("Ошибка вычисления хэша: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
                return nullptr;
            }
        }

        void ApplyAwesomeDesign()
        {
            // Устанавливаем скругленные углы для формы
            this->Region = gcnew System::Drawing::Region(CreateRoundedRectanglePath(0, 0, this->Width, this->Height, 15));

            // Добавляем эффекты при наведении на кнопку
            btnLogin->MouseEnter += gcnew EventHandler(this, &LoginForm::btnLogin_MouseEnter);
            btnLogin->MouseLeave += gcnew EventHandler(this, &LoginForm::btnLogin_MouseLeave);

            // Эффекты для текстовых полей
            txtLogin->Enter += gcnew EventHandler(this, &LoginForm::TextBox_Enter);
            txtLogin->Leave += gcnew EventHandler(this, &LoginForm::TextBox_Leave);
            txtPassword->Enter += gcnew EventHandler(this, &LoginForm::TextBox_Enter);
            txtPassword->Leave += gcnew EventHandler(this, &LoginForm::TextBox_Leave);
        }

        GraphicsPath^ CreateRoundedRectanglePath(int x, int y, int width, int height, int radius)
        {
            GraphicsPath^ path = gcnew GraphicsPath();
            path->AddArc(x, y, radius, radius, 180, 90);
            path->AddArc(x + width - radius, y, radius, radius, 270, 90);
            path->AddArc(x + width - radius, y + height - radius, radius, radius, 0, 90);
            path->AddArc(x, y + height - radius, radius, radius, 90, 90);
            path->CloseFigure();
            return path;
        }

        // НОВЫЙ МЕТОД: Инициализация счетчика попыток
        void InitializeLoginAttempts()
        {
            failedAttempts = 0;
            isAccountLocked = false;
            lockUntil = DateTime::MinValue;
            UpdateAttemptsDisplay();
        }

        // НОВЫЙ МЕТОД: Обновление отображения счетчика попыток
        void UpdateAttemptsDisplay()
        {
            if (failedAttempts > 0)
            {
                lblAttempts->Visible = true;
                lblAttempts->Text = String::Format("Неудачных попыток: {0}/5", failedAttempts);

                if (failedAttempts >= 3)
                {
                    lblAttempts->ForeColor = Color::FromArgb(241, 196, 15); // Желтый
                }
                if (failedAttempts >= 5)
                {
                    lblAttempts->ForeColor = Color::FromArgb(231, 76, 60); // Красный
                }
            }
            else
            {
                lblAttempts->Visible = false;
            }
        }

        // НОВЫЙ МЕТОД: Блокировка аккаунта
        void LockAccount()
        {
            isAccountLocked = true;
            lockUntil = DateTime::Now.AddMinutes(15); // Блокировка на 15 минут
            lockTimer->Start();

            // Блокируем элементы ввода
            txtLogin->Enabled = false;
            txtPassword->Enabled = false;
            btnLogin->Enabled = false;
            btnLogin->Text = L"🔒 ЗАБЛОКИРОВАНО";
            btnLogin->BackColor = Color::FromArgb(108, 117, 125); // Серый

            UpdateLockTimerDisplay();
        }

        // НОВЫЙ МЕТОД: Разблокировка аккаунта
        void UnlockAccount()
        {
            isAccountLocked = false;
            lockTimer->Stop();

            // Разблокируем элементы ввода
            txtLogin->Enabled = true;
            txtPassword->Enabled = true;
            btnLogin->Enabled = true;
            btnLogin->Text = L"🚀 ПРОДОЛЖИТЬ";
            btnLogin->BackColor = Color::FromArgb(0, 122, 204);

            lblLockTimer->Visible = false;
            lblAttempts->Visible = false;

            // Очищаем поля
            txtPassword->Text = "";
            txtPassword->Focus();
        }

        // НОВЫЙ МЕТОД: Обновление отображения таймера блокировки
        void UpdateLockTimerDisplay()
        {
            if (isAccountLocked)
            {
                TimeSpan remaining = lockUntil - DateTime::Now;
                if (remaining.TotalSeconds > 0)
                {
                    lblLockTimer->Visible = true;
                    lblLockTimer->Text = String::Format("Блокировка снята через: {0:00}:{1:00}",
                        remaining.Minutes, remaining.Seconds);
                }
                else
                {
                    UnlockAccount();
                }
            }
        }

        // НОВЫЙ МЕТОД: Проверка блокировки аккаунта
        bool CheckAccountLocked()
        {
            if (isAccountLocked)
            {
                if (DateTime::Now >= lockUntil)
                {
                    UnlockAccount();
                    return false;
                }
                return true;
            }
            return false;
        }

        // НОВЫЙ МЕТОД: Обработчик таймера блокировки
        System::Void lockTimer_Tick(System::Object^ sender, System::EventArgs^ e)
        {
            UpdateLockTimerDisplay();
        }

    private:
        // ИСПРАВЛЕННЫЙ МЕТОД: Обработчик кнопки входа
        System::Void btnLogin_Click(System::Object^ sender, System::EventArgs^ e)
        {
            // Проверяем блокировку аккаунта
            if (CheckAccountLocked())
            {
                MessageBox::Show("Аккаунт временно заблокирован. Попробуйте позже.", "Блокировка",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            String^ login = txtLogin->Text;
            String^ password = txtPassword->Text;

            if (String::IsNullOrEmpty(login) || String::IsNullOrEmpty(password))
            {
                ShowError("Пожалуйста, заполните все поля!");
                return;
            }

            // ВАЖНО: Сначала проверяем логин, чтобы избежать ненужного хэширования
            if (login != "Admin")
            {
                failedAttempts++;
                UpdateAttemptsDisplay();
                ShowError("Неверный логин!");

                if (failedAttempts >= 5)
                {
                    LockAccount();
                    MessageBox::Show("Слишком много неудачных попыток! Аккаунт заблокирован на 15 минут.",
                        "Блокировка аккаунта", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                }
                return;
            }

            try
            {
                // ДИАГНОСТИКА: Показываем информацию в MessageBox для отладки
                String^ debugInfo = "=== ДЕБАГ ИНФОРМАЦИЯ ===\n";
                debugInfo += "Логин: " + login + "\n";
                debugInfo += "Пароль (открытый): " + password + "\n";

                // Вычисляем хэш пароля
                String^ passwordHash = ComputeSHA256Hash(password);

                debugInfo += "Вычисленный хэш: " + passwordHash + "\n";
                debugInfo += "Длина хэша: " + (passwordHash != nullptr ? passwordHash->Length.ToString() : "null") + "\n";

                if (passwordHash == nullptr)
                {
                    ShowError("Ошибка при вычислении хэша пароля!");
                    return;
                }

                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                debugInfo += "Строка подключения: " + connStr + "\n";
                conn->Open();
                debugInfo += "Подключение успешно\n";

                // ДИАГНОСТИКА: Проверим, что есть в таблице
                String^ checkQuery = "SELECT пароль FROM \"Parol\"";
                OdbcCommand^ checkCmd = gcnew OdbcCommand(checkQuery, conn);
                OdbcDataReader^ reader = checkCmd->ExecuteReader();

                if (reader->Read())
                {
                    String^ dbHash = reader->GetString(0);
                    debugInfo += "Хэш из БД: " + dbHash + "\n";
                    debugInfo += "Длина хэша из БД: " + dbHash->Length + "\n";
                    debugInfo += "Совпадают? " + (passwordHash == dbHash) + "\n";
                    debugInfo += "Совпадают (без учета регистра)? " + passwordHash->Equals(dbHash, StringComparison::OrdinalIgnoreCase) + "\n";

                    // Для отладки покажем эту информацию
                    MessageBox::Show(debugInfo, "Отладочная информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
                else
                {
                    debugInfo += "В таблице Parol нет записей!\n";
                    MessageBox::Show(debugInfo, "Отладочная информация", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                }
                reader->Close();

                // ИСПРАВЛЕННЫЙ ЗАПРОС: сравниваем хэш пароля без учета регистра
                String^ query = "SELECT COUNT(*) FROM \"Parol\" WHERE LOWER(пароль) = LOWER(?);";
                debugInfo += "SQL запрос: " + query + "\n";
                debugInfo += "Параметр: " + passwordHash + "\n";

                OdbcCommand^ cmd = gcnew OdbcCommand(query, conn);
                cmd->Parameters->AddWithValue("пароль", passwordHash);

                int count = Convert::ToInt32(cmd->ExecuteScalar());
                debugInfo += "Найдено записей: " + count + "\n";

                if (count > 0)
                {
                    // Успешная авторизация
                    debugInfo += "=== АВТОРИЗАЦИЯ УСПЕШНА ===\n";
                    MessageBox::Show(debugInfo, "Успешная авторизация", MessageBoxButtons::OK, MessageBoxIcon::Information);

                    failedAttempts = 0; // Сбрасываем счетчик
                    UpdateAttemptsDisplay();

                    btnLogin->Text = L"✅ УСПЕХ!";
                    btnLogin->BackColor = Color::FromArgb(46, 204, 113);
                    Application::DoEvents();
                    System::Threading::Thread::Sleep(500);

                    this->Hide();
                    AdminPanel^ adminForm = gcnew AdminPanel();
                    adminForm->ShowDialog();
                    this->Close();
                }
                else
                {
                    // Неверный пароль
                    debugInfo += "=== НЕВЕРНЫЙ ПАРОЛЬ ===\n";
                    MessageBox::Show(debugInfo, "Неверный пароль", MessageBoxButtons::OK, MessageBoxIcon::Error);

                    failedAttempts++;
                    UpdateAttemptsDisplay();

                    if (failedAttempts >= 5)
                    {
                        LockAccount();
                        MessageBox::Show("Слишком много неудачных попыток! Аккаунт заблокирован на 15 минут.",
                            "Блокировка аккаунта", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    }
                    else
                    {
                        ShowError("Неверный пароль!");
                    }
                }

                conn->Close();
            }
            catch (Exception^ ex)
            {
                String^ errorInfo = "[ERROR] Исключение: " + ex->Message + "\n";
                errorInfo += "[ERROR] Стек вызовов: " + ex->StackTrace;
                MessageBox::Show(errorInfo, "Критическая ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                ShowError("Ошибка подключения: " + ex->Message);
            }
        }

        // ИСПРАВЛЕННЫЙ МЕТОД: Отображение ошибки
        void ShowError(String^ message)
        {
            btnLogin->Text = L"❌ ОШИБКА";
            btnLogin->BackColor = Color::FromArgb(231, 76, 60);
            MessageBox::Show(message, "Ошибка авторизации",
                MessageBoxButtons::OK, MessageBoxIcon::Error);

            // Возвращаем нормальный вид кнопке
            btnLogin->Text = L"🚀 ПРОДОЛЖИТЬ";
            btnLogin->BackColor = Color::FromArgb(0, 122, 204);

            // Очищаем поле пароля и устанавливаем фокус
            txtPassword->Text = "";
            txtPassword->Focus();
        }

    private:
        // Эффекты при наведении (остаются без изменений)
        System::Void btnLogin_MouseEnter(System::Object^ sender, System::EventArgs^ e)
        {
            if (!isAccountLocked)
            {
                btnLogin->BackColor = Color::FromArgb(0, 150, 255);
                btnLogin->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, System::Drawing::FontStyle::Bold);
            }
        }

        System::Void btnLogin_MouseLeave(System::Object^ sender, System::EventArgs^ e)
        {
            if (!isAccountLocked)
            {
                btnLogin->BackColor = Color::FromArgb(0, 122, 204);
                btnLogin->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, System::Drawing::FontStyle::Bold);
            }
        }

        // Эффекты для текстовых полей (остаются без изменений)
        System::Void TextBox_Enter(System::Object^ sender, System::EventArgs^ e)
        {
            TextBox^ tb = safe_cast<TextBox^>(sender);
            Panel^ parent = safe_cast<Panel^>(tb->Parent);
            parent->BackColor = Color::FromArgb(0, 122, 204);
        }

        System::Void TextBox_Leave(System::Object^ sender, System::EventArgs^ e)
        {
            TextBox^ tb = safe_cast<TextBox^>(sender);
            Panel^ parent = safe_cast<Panel^>(tb->Parent);
            parent->BackColor = Color::FromArgb(45, 45, 48);
        }

        // Управление окном (остается без изменений)
        System::Void panelHeader_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
        {
            dragging = true;
            dragCursorPoint = Cursor->Position;
            dragFormPoint = this->Location;
        }

        System::Void panelHeader_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
        {
            if (dragging)
            {
                Point dif = Point(Point::Subtract(Cursor->Position, System::Drawing::Size(dragCursorPoint)));
                this->Location = Point(Point::Add(dragFormPoint, System::Drawing::Size(dif)));
            }
        }

        System::Void panelHeader_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
        {
            dragging = false;
        }

        System::Void btnClose_Click(System::Object^ sender, System::EventArgs^ e)
        {
            Application::Exit();
        }

        System::Void btnMinimize_Click(System::Object^ sender, System::EventArgs^ e)
        {
            this->WindowState = FormWindowState::Minimized;
        }
    };
}