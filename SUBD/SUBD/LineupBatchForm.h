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

    public ref class BatchForm : public System::Windows::Forms::Form
    {
    public:
        enum class FormMode { Lineup, Training };

        BatchForm(FormMode mode, int recordId, String^ recordInfo, String^ connStr)
        {
            this->mode = mode;
            this->recordId = recordId;
            this->recordInfo = recordInfo;
            this->connStr = connStr;
            this->injuredPlayers = gcnew List<String^>();
            InitializeComponent();
            LoadInjuredPlayers(); // Загружаем список травмированных игроков
            LoadPlayers();
            ApplyModernDesign();
            UpdateFormForMode();
            ShowInjuryWarningIfNeeded(); // Показываем предупреждение если есть травмированные
        }

    private:
        FormMode mode;
        int recordId;
        String^ recordInfo;
        String^ connStr;
        ListBox^ lstAvailablePlayers;
        ListBox^ lstSelectedPlayers;
        Button^ btnAddToList;
        Button^ btnRemoveFromList;
        Button^ btnSave;
        Button^ btnCancel;
        ComboBox^ cmbPosition;
        ComboBox^ cmbStatus;
        Label^ lblRecordInfo;
        Label^ lblAvailablePlayers;
        Label^ lblSelectedPlayers;
        Label^ lblPosition;
        Label^ lblStatus;
        DataTable^ playersTable;
        List<String^>^ injuredPlayers; // Список игроков с активными травмами
        Label^ lblInjuryWarning;
        Dictionary<String^, String^>^ playerOriginalNames; // Сопоставление отображаемого имени с оригинальным

        void InitializeComponent()
        {
            String^ formTitle = (mode == FormMode::Lineup) ? "Массовый ввод состава" : "Массовый ввод участников тренировки";
            this->Text = formTitle;
            this->Size = Drawing::Size(800, 650); // Увеличили высоту для предупреждения
            this->StartPosition = FormStartPosition::CenterParent;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;

            // Заголовок с информацией
            lblRecordInfo = gcnew Label();
            lblRecordInfo->Text = GetRecordInfoText();
            lblRecordInfo->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
            lblRecordInfo->Location = Point(20, 20);
            lblRecordInfo->Size = Drawing::Size(750, 25);
            this->Controls->Add(lblRecordInfo);

            // Предупреждение о травмах
            lblInjuryWarning = gcnew Label();
            lblInjuryWarning->Text = "";
            lblInjuryWarning->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Regular);
            lblInjuryWarning->ForeColor = Color::DarkRed;
            lblInjuryWarning->Location = Point(20, 50);
            lblInjuryWarning->Size = Drawing::Size(750, 40);
            lblInjuryWarning->Visible = false;
            this->Controls->Add(lblInjuryWarning);

            // Доступные игроки
            lblAvailablePlayers = gcnew Label();
            lblAvailablePlayers->Text = "Доступные игроки:";
            lblAvailablePlayers->Location = Point(20, 100);
            lblAvailablePlayers->Size = Drawing::Size(200, 20);
            this->Controls->Add(lblAvailablePlayers);

            lstAvailablePlayers = gcnew ListBox();
            lstAvailablePlayers->Location = Point(20, 125);
            lstAvailablePlayers->Size = Drawing::Size(300, 200);
            lstAvailablePlayers->SelectionMode = SelectionMode::MultiSimple;
            lstAvailablePlayers->DrawMode = DrawMode::OwnerDrawFixed;
            lstAvailablePlayers->DrawItem += gcnew DrawItemEventHandler(this, &BatchForm::lstAvailablePlayers_DrawItem);
            this->Controls->Add(lstAvailablePlayers);

            // Выбранные игроки
            lblSelectedPlayers = gcnew Label();
            lblSelectedPlayers->Text = GetSelectedListText();
            lblSelectedPlayers->Location = Point(470, 100);
            lblSelectedPlayers->Size = Drawing::Size(200, 20);
            this->Controls->Add(lblSelectedPlayers);

            lstSelectedPlayers = gcnew ListBox();
            lstSelectedPlayers->Location = Point(470, 125);
            lstSelectedPlayers->Size = Drawing::Size(300, 200);
            lstSelectedPlayers->DrawMode = DrawMode::OwnerDrawFixed;
            lstSelectedPlayers->DrawItem += gcnew DrawItemEventHandler(this, &BatchForm::lstSelectedPlayers_DrawItem);
            this->Controls->Add(lstSelectedPlayers);

            // Кнопки перемещения
            btnAddToList = gcnew Button();
            btnAddToList->Text = "Добавить";
            btnAddToList->Location = Point(340, 150);
            btnAddToList->Size = Drawing::Size(120, 30);
            btnAddToList->Click += gcnew EventHandler(this, &BatchForm::btnAddToList_Click);
            this->Controls->Add(btnAddToList);

            btnRemoveFromList = gcnew Button();
            btnRemoveFromList->Text = "Удалить";
            btnRemoveFromList->Location = Point(340, 190);
            btnRemoveFromList->Size = Drawing::Size(120, 30);
            btnRemoveFromList->Click += gcnew EventHandler(this, &BatchForm::btnRemoveFromList_Click);
            this->Controls->Add(btnRemoveFromList);

            // Позиция (только для составов)
            lblPosition = gcnew Label();
            lblPosition->Text = "Позиция для выбранных игроков:";
            lblPosition->Location = Point(20, 340);
            lblPosition->Size = Drawing::Size(250, 20);
            this->Controls->Add(lblPosition);

            cmbPosition = gcnew ComboBox();
            cmbPosition->Location = Point(20, 365);
            cmbPosition->Size = Drawing::Size(250, 25);
            cmbPosition->DropDownStyle = ComboBoxStyle::DropDownList;
            array<String^>^ positions = {
                "Вратарь", "Центральный защитник", "Левый защитник",
                "Правый защитник", "Опорный полузащитник", "Центральный полузащитник",
                "Атакующий полузащитник", "Левый вингер", "Правый вингер", "Нападающий"
            };
            cmbPosition->Items->AddRange(positions);
            if (positions->Length > 0) cmbPosition->SelectedIndex = 0;
            this->Controls->Add(cmbPosition);

            // Статус (только для составов)
            lblStatus = gcnew Label();
            lblStatus->Text = "Статус для выбранных игроков:";
            lblStatus->Location = Point(300, 340);
            lblStatus->Size = Drawing::Size(250, 20);
            this->Controls->Add(lblStatus);

            cmbStatus = gcnew ComboBox();
            cmbStatus->Location = Point(300, 365);
            cmbStatus->Size = Drawing::Size(250, 25);
            cmbStatus->DropDownStyle = ComboBoxStyle::DropDownList;
            array<String^>^ statuses = { "Основной", "Запасной" };
            cmbStatus->Items->AddRange(statuses);
            if (statuses->Length > 0) cmbStatus->SelectedIndex = 0;
            this->Controls->Add(cmbStatus);

            // Кнопки сохранения/отмены
            btnSave = gcnew Button();
            btnSave->Text = GetSaveButtonText();
            btnSave->Location = Point(470, 400);
            btnSave->Size = Drawing::Size(140, 40);
            btnSave->Click += gcnew EventHandler(this, &BatchForm::btnSave_Click);
            this->Controls->Add(btnSave);

            btnCancel = gcnew Button();
            btnCancel->Text = "Отмена";
            btnCancel->Location = Point(620, 400);
            btnCancel->Size = Drawing::Size(140, 40);
            btnCancel->Click += gcnew EventHandler(this, &BatchForm::btnCancel_Click);
            this->Controls->Add(btnCancel);

            // Подсказка
            Label^ lblHint = gcnew Label();
            lblHint->Text = GetHintText();
            lblHint->Location = Point(20, 420);
            lblHint->Size = Drawing::Size(750, 40);
            lblHint->ForeColor = Color::DarkBlue;
            lblHint->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Italic);
            this->Controls->Add(lblHint);
        }

        String^ GetRecordInfoText()
        {
            return (mode == FormMode::Lineup) ?
                "Матч: " + recordInfo :
                "Тренировка: " + recordInfo;
        }

        String^ GetSelectedListText()
        {
            return (mode == FormMode::Lineup) ?
                "Состав на матч:" :
                "Участники тренировки:";
        }

        String^ GetSaveButtonText()
        {
            return (mode == FormMode::Lineup) ?
                "Сохранить состав" :
                "Сохранить участников";
        }

        String^ GetHintText()
        {
            if (mode == FormMode::Lineup) {
                return "Подсказка: Выберите несколько игроков (Ctrl+клик) и назначьте им позицию/статус. Игроки с активными травмами помечены красным.";
            }
            else {
                return "Подсказка: Выберите несколько игроков (Ctrl+клик) для добавления в тренировку. Игроки с активными травмами помечены красным.";
            }
        }

        void UpdateFormForMode()
        {
            if (mode == FormMode::Training) {
                // Скрываем элементы, специфичные для составов
                lblPosition->Visible = false;
                cmbPosition->Visible = false;
                lblStatus->Visible = false;
                cmbStatus->Visible = false;

                // Перемещаем кнопки сохранения выше
                btnSave->Location = Point(470, 340);
                btnCancel->Location = Point(620, 340);

                // Перемещаем подсказку выше
                Controls[Controls->Count - 1]->Location = Point(20, 380);
            }
        }

        void ApplyModernDesign()
        {
            this->BackColor = Color::FromArgb(245, 247, 250);
            this->Font = gcnew Drawing::Font("Segoe UI", 9);

            // Стилизация кнопок
            ApplyButtonStyle(btnAddToList, Color::FromArgb(41, 128, 185));
            ApplyButtonStyle(btnRemoveFromList, Color::FromArgb(231, 76, 60));
            ApplyButtonStyle(btnSave, Color::FromArgb(46, 204, 113));
            ApplyButtonStyle(btnCancel, Color::FromArgb(108, 117, 125));

            // Стилизация списков
            lstAvailablePlayers->BackColor = Color::White;
            lstSelectedPlayers->BackColor = Color::White;
            lstAvailablePlayers->Font = gcnew Drawing::Font("Segoe UI", 9);
            lstSelectedPlayers->Font = gcnew Drawing::Font("Segoe UI", 9);

            // Стилизация комбобоксов
            cmbPosition->BackColor = Color::White;
            cmbStatus->BackColor = Color::White;
        }

        void ApplyButtonStyle(Button^ button, Color color)
        {
            button->FlatStyle = FlatStyle::Flat;
            button->FlatAppearance->BorderSize = 0;
            button->BackColor = color;
            button->ForeColor = Color::White;
            button->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            button->Cursor = Cursors::Hand;
        }

        // Метод для загрузки списка игроков с активными травмами
        void LoadInjuredPlayers()
        {
            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);
                OdbcCommand^ cmd = gcnew OdbcCommand(
                    "SELECT DISTINCT player_name FROM injuries WHERE status = 'Актуальна' AND (date_end IS NULL OR date_end >= ?);",
                    conn);
                cmd->Parameters->AddWithValue("current_date", DateTime::Now.Date);

                conn->Open();
                OdbcDataReader^ reader = cmd->ExecuteReader();

                while (reader->Read()) {
                    injuredPlayers->Add(reader->GetString(0));
                }

                reader->Close();
                conn->Close();
            }
            catch (Exception^ ex) {
                System::Diagnostics::Debug::WriteLine("Ошибка загрузки списка травм: " + ex->Message);
            }
        }

        // Показать предупреждение о травмах если нужно
        void ShowInjuryWarningIfNeeded()
        {
            if (injuredPlayers->Count > 0) {
                lblInjuryWarning->Text = "Внимание: " + injuredPlayers->Count.ToString() +
                    " игроков имеют активные травмы. Они помечены красным и не могут быть добавлены.";
                lblInjuryWarning->Visible = true;
            }
        }

        // Проверить, имеет ли игрок активную травму
        bool PlayerHasActiveInjury(String^ playerName)
        {
            // Извлекаем только имя из строки (убираем позицию в скобках)
            String^ cleanPlayerName = playerName;
            int bracketPos = cleanPlayerName->IndexOf('(');
            if (bracketPos > 0) {
                cleanPlayerName = cleanPlayerName->Substring(0, bracketPos)->Trim();
            }

            // Также убираем маркер травмы если есть
            if (cleanPlayerName->StartsWith("[ТРАВМА] ")) {
                cleanPlayerName = cleanPlayerName->Substring(9)->Trim();
            }

            for (int i = 0; i < injuredPlayers->Count; i++) {
                if (cleanPlayerName == injuredPlayers[i]) {
                    return true;
                }
            }
            return false;
        }

        // Метод для загрузки игроков с исключением травмированных
        void LoadPlayers()
        {
            playerOriginalNames = gcnew Dictionary<String^, String^>();

            try {
                OdbcConnection^ conn = gcnew OdbcConnection(connStr);

                // Загружаем игроков
                OdbcDataAdapter^ da = gcnew OdbcDataAdapter(
                    "SELECT p.id, p.\"Имя\", p.\"Позиция\", "
                    "CASE WHEN i.player_name IS NOT NULL THEN 1 ELSE 0 END as has_injury "
                    "FROM \"Player\" p "
                    "LEFT JOIN ("
                    "    SELECT DISTINCT player_name FROM injuries "
                    "    WHERE status = 'Актуальна' AND (date_end IS NULL OR date_end >= ?)"
                    ") i ON p.\"Имя\" = i.player_name "
                    "ORDER BY p.\"Имя\";", conn);

                da->SelectCommand->Parameters->AddWithValue("current_date", DateTime::Now.Date);
                playersTable = gcnew DataTable();
                da->Fill(playersTable);

                lstAvailablePlayers->Items->Clear();
                for (int i = 0; i < playersTable->Rows->Count; i++) {
                    DataRow^ row = playersTable->Rows[i];
                    String^ playerName = row["Имя"]->ToString();
                    String^ playerPosition = row->IsNull("Позиция") ? "без позиции" : row["Позиция"]->ToString();
                    bool hasInjury = Convert::ToBoolean(row["has_injury"]);

                    String^ displayName = String::Format("{0} ({1})", playerName, playerPosition);
                    String^ originalName = playerName; // Сохраняем оригинальное имя

                    if (hasInjury) {
                        displayName = "[ТРАВМА] " + displayName;
                    }

                    lstAvailablePlayers->Items->Add(displayName);
                    playerOriginalNames[displayName] = originalName;
                }

                conn->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка загрузки игроков: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        // Отрисовка элементов в списке доступных игроков
        void lstAvailablePlayers_DrawItem(Object^ sender, DrawItemEventArgs^ e)
        {
            e->DrawBackground();

            if (e->Index >= 0 && e->Index < lstAvailablePlayers->Items->Count) {
                String^ itemText = lstAvailablePlayers->Items[e->Index]->ToString();
                Brush^ textBrush;

                // Определяем цвет текста
                if (itemText->StartsWith("[ТРАВМА] ")) {
                    textBrush = gcnew SolidBrush(Color::Red); // Красный для травмированных
                }
                else {
                    textBrush = gcnew SolidBrush(e->ForeColor); // Обычный цвет для здоровых
                }

                // Рисуем текст
                e->Graphics->DrawString(itemText, e->Font, textBrush, e->Bounds);

                // Рисуем фокус если элемент выбран
                e->DrawFocusRectangle();

                delete textBrush;
            }
        }

        // Отрисовка элементов в списке выбранных игроков
        void lstSelectedPlayers_DrawItem(Object^ sender, DrawItemEventArgs^ e)
        {
            e->DrawBackground();

            if (e->Index >= 0 && e->Index < lstSelectedPlayers->Items->Count) {
                String^ itemText = lstSelectedPlayers->Items[e->Index]->ToString();
                Brush^ textBrush;

                // Проверяем содержит ли текст метку травмы
                if (itemText->Contains("[ТРАВМА]")) {
                    textBrush = gcnew SolidBrush(Color::Red); // Красный для травмированных
                }
                else {
                    textBrush = gcnew SolidBrush(e->ForeColor); // Обычный цвет для здоровых
                }

                // Рисуем текст
                e->Graphics->DrawString(itemText, e->Font, textBrush, e->Bounds);

                // Рисуем фокус если элемент выбран
                e->DrawFocusRectangle();

                delete textBrush;
            }
        }

        void btnAddToList_Click(Object^ sender, EventArgs^ e)
        {
            if (lstAvailablePlayers->SelectedItems->Count == 0) {
                MessageBox::Show("Выберите игроков для добавления!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            if (mode == FormMode::Lineup && cmbPosition->SelectedItem == nullptr) {
                MessageBox::Show("Выберите позицию для игроков!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            List<String^>^ injuredSelectedPlayers = gcnew List<String^>();
            List<String^>^ healthySelectedPlayers = gcnew List<String^>();

            // Сначала проверяем всех выбранных игроков
            for (int i = 0; i < lstAvailablePlayers->SelectedItems->Count; i++) {
                String^ playerInfo = lstAvailablePlayers->SelectedItems[i]->ToString();
                if (PlayerHasActiveInjury(playerInfo)) {
                    injuredSelectedPlayers->Add(playerInfo);
                }
                else {
                    healthySelectedPlayers->Add(playerInfo);
                }
            }

            // Показываем предупреждение о травмированных игроках
            if (injuredSelectedPlayers->Count > 0) {
                String^ warningMessage = "Следующие игроки имеют активные травмы и не могут быть добавлены:\n\n";
                for (int i = 0; i < injuredSelectedPlayers->Count; i++) {
                    String^ player = injuredSelectedPlayers[i];
                    // Убираем метку травмы если есть
                    String^ cleanPlayerName = player;
                    if (cleanPlayerName->StartsWith("[ТРАВМА] ")) {
                        cleanPlayerName = cleanPlayerName->Substring(9);
                    }
                    warningMessage += "• " + cleanPlayerName + "\n";
                }
                warningMessage += "\nИх нельзя добавить в состав/тренировку.";

                MessageBox::Show(warningMessage, "Игроки с травмами",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
            }

            // Добавляем только здоровых игроков
            for (int i = 0; i < healthySelectedPlayers->Count; i++) {
                String^ playerInfo = healthySelectedPlayers[i];
                String^ itemInfo;

                // Убираем метку травмы если есть
                String^ cleanPlayerInfo = playerInfo;
                if (cleanPlayerInfo->StartsWith("[ТРАВМА] ")) {
                    cleanPlayerInfo = cleanPlayerInfo->Substring(9);
                }

                if (mode == FormMode::Lineup) {
                    String^ position = cmbPosition->SelectedItem->ToString();
                    String^ status = cmbStatus->SelectedItem->ToString();
                    itemInfo = String::Format("{0} | {1} | {2}", cleanPlayerInfo, position, status);
                }
                else {
                    // Для тренировок добавляем просто имя игрока с позицией в скобках
                    itemInfo = cleanPlayerInfo;
                }
                lstSelectedPlayers->Items->Add(itemInfo);
            }

            // Показываем информационное сообщение если были добавлены игроки
            if (healthySelectedPlayers->Count > 0 && injuredSelectedPlayers->Count > 0) {
                MessageBox::Show(String::Format("Добавлено {0} здоровых игроков. {1} игроков с травмами пропущены.",
                    healthySelectedPlayers->Count, injuredSelectedPlayers->Count),
                    "Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            else if (healthySelectedPlayers->Count > 0) {
                MessageBox::Show(String::Format("Добавлено {0} здоровых игроков.", healthySelectedPlayers->Count),
                    "Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void btnRemoveFromList_Click(Object^ sender, EventArgs^ e)
        {
            if (lstSelectedPlayers->SelectedItems->Count == 0) {
                MessageBox::Show("Выберите элементы для удаления!", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            // Удаляем в обратном порядке чтобы избежать проблем с индексами
            for (int i = lstSelectedPlayers->SelectedItems->Count - 1; i >= 0; i--) {
                int index = lstSelectedPlayers->SelectedIndices[i];
                lstSelectedPlayers->Items->RemoveAt(index);
            }
        }

        void btnSave_Click(Object^ sender, EventArgs^ e)
        {
            if (lstSelectedPlayers->Items->Count == 0) {
                MessageBox::Show("Список пуст! Добавьте игроков.", "Внимание",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            // Финализируем проверку перед сохранением
            List<String^>^ selectedInjuredPlayers = gcnew List<String^>();

            // Проверяем для обоих режимов
            for (int i = 0; i < lstSelectedPlayers->Items->Count; i++) {
                String^ playerInfo = lstSelectedPlayers->Items[i]->ToString();
                String^ playerName = playerInfo;

                // Разбираем в зависимости от режима
                if (mode == FormMode::Lineup) {
                    array<String^>^ parts = playerInfo->Split('|');
                    if (parts->Length >= 3) {
                        playerName = parts[0]->Trim();
                    }
                }

                // Убираем позицию в скобках если есть
                int bracketPos = playerName->IndexOf('(');
                if (bracketPos > 0) {
                    playerName = playerName->Substring(0, bracketPos)->Trim();
                }

                // Убираем метку травмы если есть
                if (playerName->StartsWith("[ТРАВМА] ")) {
                    playerName = playerName->Substring(9)->Trim();
                }

                // Проверяем есть ли у игрока активная травма
                if (PlayerHasActiveInjury(playerName)) {
                    selectedInjuredPlayers->Add(playerName);
                }
            }

            if (selectedInjuredPlayers->Count > 0) {
                String^ errorMessage = "Ошибка! В списке все еще есть игроки с активными травмами:\n\n";
                for (int i = 0; i < selectedInjuredPlayers->Count; i++) {
                    String^ player = selectedInjuredPlayers[i];
                    errorMessage += "• " + player + "\n";
                }
                errorMessage += "\nУдалите этих игроков перед сохранением.";

                MessageBox::Show(errorMessage, "Ошибка сохранения",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            try {
                if (mode == FormMode::Lineup) {
                    SaveLineup();
                }
                else {
                    SaveTrainingParticipants();
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка сохранения: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void SaveLineup()
        {
            OdbcConnection^ conn = gcnew OdbcConnection(connStr);
            conn->Open();

            try {
                // Сначала удаляем старый состав для этого матча
                OdbcCommand^ deleteCmd = gcnew OdbcCommand(
                    "DELETE FROM matchlineups WHERE match_id = ?;", conn);
                deleteCmd->Parameters->AddWithValue("match_id", recordId);
                deleteCmd->ExecuteNonQuery();

                // Добавляем новый состав
                OdbcCommand^ insertCmd = gcnew OdbcCommand(
                    "INSERT INTO matchlineups (match_id, player_name, position, is_starter) VALUES (?, ?, ?, ?);", conn);
                insertCmd->Parameters->Add(gcnew OdbcParameter("match_id", OdbcType::Int));
                insertCmd->Parameters->Add(gcnew OdbcParameter("player_name", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("position", OdbcType::VarChar));
                insertCmd->Parameters->Add(gcnew OdbcParameter("is_starter", OdbcType::Bit));

                int successCount = 0;
                int errorCount = 0;
                for (int i = 0; i < lstSelectedPlayers->Items->Count; i++) {
                    try {
                        String^ lineupInfo = lstSelectedPlayers->Items[i]->ToString();
                        // Парсим информацию об игроке (формат: "Имя (Позиция) | НоваяПозиция | Статус")
                        array<String^>^ parts = lineupInfo->Split('|');
                        if (parts->Length >= 3) {
                            String^ playerName = parts[0]->Trim();
                            // Убираем позицию в скобках если есть
                            int bracketPos = playerName->IndexOf('(');
                            if (bracketPos > 0) {
                                playerName = playerName->Substring(0, bracketPos)->Trim();
                            }

                            String^ position = parts[1]->Trim();
                            String^ status = parts[2]->Trim();
                            bool isStarter = (status == "Основной");

                            insertCmd->Parameters[0]->Value = recordId;
                            insertCmd->Parameters[1]->Value = playerName;
                            insertCmd->Parameters[2]->Value = position;
                            insertCmd->Parameters[3]->Value = isStarter;
                            insertCmd->ExecuteNonQuery();
                            successCount++;
                        }
                    }
                    catch (Exception^ ex) {
                        errorCount++;
                        System::Diagnostics::Debug::WriteLine("Ошибка добавления игрока: " + ex->Message);
                    }
                }

                String^ message = String::Format("Состав успешно сохранен!\nДобавлено игроков: {0}", successCount);
                if (errorCount > 0) {
                    message += String::Format("\nОшибок при добавлении: {0}", errorCount);
                }

                MessageBox::Show(message, "Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);

                this->DialogResult = Windows::Forms::DialogResult::OK;
                this->Close();
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка сохранения состава: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            finally {
                if (conn->State == ConnectionState::Open) {
                    conn->Close();
                }
            }
        }

        void SaveTrainingParticipants()
        {
            OdbcConnection^ conn = gcnew OdbcConnection(connStr);

            try {
                conn->Open();

                // Отладочная информация
                System::Diagnostics::Debug::WriteLine("Попытка сохранить участников для тренировки ID: " + recordId);

                // Сначала проверим, существует ли тренировка
                OdbcCommand^ checkCmd = gcnew OdbcCommand("SELECT COUNT(*) FROM trainings WHERE training_id = ?;", conn);
                checkCmd->Parameters->AddWithValue("training_id", recordId);
                int exists = Convert::ToInt32(checkCmd->ExecuteScalar());

                System::Diagnostics::Debug::WriteLine("Тренировка найдена: " + exists + " записей");

                if (exists == 0) {
                    // Если тренировка не найдена, попробуем найти по другим критериям
                    // Или создадим новую тренировку
                    String^ message = "Тренировка с ID " + recordId + " не найдена.\n";
                    message += "Хотите создать новую тренировку с текущей датой?";

                    if (MessageBox::Show(message, "Тренировка не найдена",
                        MessageBoxButtons::YesNo, MessageBoxIcon::Question) == Windows::Forms::DialogResult::Yes) {

                        // Создаем новую тренировку
                        OdbcCommand^ insertCmd = gcnew OdbcCommand(
                            "INSERT INTO trainings (training_date, focus_area, participants) VALUES (?, ?, ?);", conn);

                        // Используем текущую дату
                        insertCmd->Parameters->AddWithValue("training_date", DateTime::Now.Date);
                        // Пустая тема тренировки
                        insertCmd->Parameters->AddWithValue("focus_area", "Общая тренировка");
                        // Пока пустые участники
                        insertCmd->Parameters->AddWithValue("participants", "");

                        insertCmd->ExecuteNonQuery();

                        // Получаем ID новой тренировки
                        OdbcCommand^ getIdCmd = gcnew OdbcCommand("SELECT LASTVAL();", conn);
                        int newTrainingId = Convert::ToInt32(getIdCmd->ExecuteScalar());

                        System::Diagnostics::Debug::WriteLine("Создана новая тренировка с ID: " + newTrainingId);

                        // Обновляем recordId на новый ID
                        recordId = newTrainingId;
                    }
                    else {
                        conn->Close();
                        return;
                    }
                }

                // Формируем список участников
                List<String^>^ participants = gcnew List<String^>();
                for (int i = 0; i < lstSelectedPlayers->Items->Count; i++) {
                    String^ playerInfo = lstSelectedPlayers->Items[i]->ToString();

                    String^ playerName = playerInfo;

                    // Если есть вертикальная черта (|), значит это формат состава, извлекаем первую часть
                    if (playerInfo->Contains("|")) {
                        array<String^>^ parts = playerInfo->Split('|');
                        if (parts->Length > 0) {
                            playerName = parts[0]->Trim();
                        }
                    }

                    // Убираем позицию в скобках если есть
                    int bracketPos = playerName->IndexOf('(');
                    if (bracketPos > 0) {
                        playerName = playerName->Substring(0, bracketPos)->Trim();
                    }

                    // Убираем метку травмы если есть
                    if (playerName->StartsWith("[ТРАВМА] ")) {
                        playerName = playerName->Substring(9)->Trim();
                    }

                    // Добавляем только если имя не пустое
                    if (!String::IsNullOrEmpty(playerName) && !String::IsNullOrWhiteSpace(playerName)) {
                        participants->Add(playerName);
                    }
                }

                if (participants->Count == 0) {
                    MessageBox::Show("Нет участников для сохранения!", "Внимание",
                        MessageBoxButtons::OK, MessageBoxIcon::Warning);
                    conn->Close();
                    return;
                }

                // Объединяем имена в одну строку
                String^ participantsString = String::Join(", ", participants->ToArray());

                System::Diagnostics::Debug::WriteLine("Список участников: " + participantsString);

                // Проверяем длину строки
                if (participantsString->Length > 4000) {
                    MessageBox::Show("Слишком много участников! Максимальная длина списка превышена.", "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                    conn->Close();
                    return;
                }

                // Обновляем запись тренировки
                OdbcCommand^ updateCmd = gcnew OdbcCommand("UPDATE trainings SET participants = ? WHERE training_id = ?;", conn);

                OdbcParameter^ param1 = gcnew OdbcParameter("participants", OdbcType::VarChar, participantsString->Length);
                param1->Value = participantsString;
                updateCmd->Parameters->Add(param1);

                OdbcParameter^ param2 = gcnew OdbcParameter("training_id", OdbcType::Int);
                param2->Value = recordId;
                updateCmd->Parameters->Add(param2);

                int rowsAffected = updateCmd->ExecuteNonQuery();

                System::Diagnostics::Debug::WriteLine("Затронуто строк: " + rowsAffected);

                if (rowsAffected > 0) {
                    MessageBox::Show(String::Format("Участники тренировки успешно сохранены! Добавлено {0} игроков.", participants->Count),
                        "Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
                    this->DialogResult = Windows::Forms::DialogResult::OK;
                    this->Close();
                }
                else {
                    MessageBox::Show("Не удалось сохранить участников тренировки.", "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
            catch (Exception^ ex) {
                System::Diagnostics::Debug::WriteLine("Ошибка: " + ex->Message);
                System::Diagnostics::Debug::WriteLine("Stack Trace: " + ex->StackTrace);

                MessageBox::Show("Ошибка сохранения участников тренировки: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            finally {
                if (conn->State == ConnectionState::Open) {
                    conn->Close();
                }
            }
        }

        void btnCancel_Click(Object^ sender, EventArgs^ e)
        {
            this->DialogResult = Windows::Forms::DialogResult::Cancel;
            this->Close();
        }
    };
}