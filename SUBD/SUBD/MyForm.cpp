#include "MyForm.h"
#using <System.Data.dll>
using namespace System::Data;
using namespace System::Data::Odbc;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

[STAThread]
void main(array<String^>^ arg)
{
    Application::EnableVisualStyles;
    Application::SetCompatibleTextRenderingDefault(false);
    SUBD::MyForm form;
    Application::Run(% form);
}