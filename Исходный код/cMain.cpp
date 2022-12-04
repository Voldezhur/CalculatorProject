#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Calculator", wxPoint(30, 30), wxSize(800, 600))
{	
	// set font to a nice one
	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	// create array of buttons
	btn = new wxButton * [nFieldWidth * nFieldHeight];
	// create grid for buttons
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

	// create textbox for calc output
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(150, 50));
	m_txt1->SetFont(font);
	

	// create buttons and put buttons into grid sizer and link buttons to function
	int t = 1, c = 0, operandCount = 0;
	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			// number keys
			if (c < 3) 
			{
				btn[y * nFieldWidth + x] = new wxButton(this, 10000 + t);
				btn[y * nFieldWidth + x]->SetFont(font);
				
				// numbers 0 - 9
				if (t <= 9 || t == 11)
				{
					std::string numberLabel = (t < 10 ? std::to_string(t) : "0");
					btn[y * nFieldWidth + x]->SetLabel(numberLabel);
					btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onButtonClicked, this);
					btn[y * nFieldWidth + x]->SetBackgroundColour(wxColor(200, 200, 200));
				}

				// enter key
				else if (t == 10)
				{
					std::string numberLabel = ("Enter");
					btn[y * nFieldWidth + x]->SetLabel(numberLabel);
					btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onEnterButtonClicked, this);
					btn[y * nFieldWidth + x]->SetBackgroundColour(wxColor(150, 150, 150));
				}
				
				// clear key
				else if (t == 12)
				{
					std::string numberLabel = ("Clear");
					btn[y * nFieldWidth + x]->SetLabel(numberLabel);
					btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onClearButtonClicked, this);
					btn[y * nFieldWidth + x]->SetBackgroundColour(wxColor(150, 150, 150));
				}

				t++;
				c++;
			}

			// operand keys
			else
			{
				btn[y * nFieldWidth + x] = new wxButton(this, 10020 + operandCount);
				btn[y * nFieldWidth + x]->SetFont(font);
				btn[y * nFieldWidth + x]->SetBackgroundColour(wxColor(150, 150, 150));
				
				std::string operandLabel;
				switch (operandCount)
				{
				case 0:
					operandLabel = "+";
					break;
				case 1:
					operandLabel = "-";
					break;
				case 2:
					operandLabel = "*";
					break;
				case 3:
					operandLabel = "/";
					break;
				}
				
				operandCount++;
				c = 0;

				btn[y * nFieldWidth + x]->SetLabel(operandLabel);
				btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onOperandButtonClicked, this);
			}

			// add into grid and link to function
			grid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);
		}
	}

	grid->Layout();
	
	// create main sizer
	wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(m_txt1, 1, wxEXPAND | wxALL, 10);
	mainSizer->Add(grid, 1, wxEXPAND | wxALL, 10);
	
	this->SetSizer(mainSizer);
	SetSizerAndFit(mainSizer);
}


cMain::~cMain()
{
	delete[] btn;
}

void cMain::onButtonClicked(wxCommandEvent& evt)
{
	int numberId = evt.GetId() - 10000;
	std::string numberLabel = std::to_string(numberId);
	m_txt1->AppendText((numberId <= 9 ? numberLabel : "0"));
	evt.Skip();
}

void cMain::onOperandButtonClicked(wxCommandEvent& evt)
{
	std::string operandLabel;
	switch (evt.GetId() - 10020)
	{
	case 0:
		operandLabel = "+";
		break;
	case 1:
		operandLabel = "-";
		break;
	case 2:
		operandLabel = "*";
		break;
	case 3:
		operandLabel = "/";
		break;
	}
	
	m_txt1->AppendText(operandLabel);
}

void cMain::onClearButtonClicked(wxCommandEvent& evt)
{
	m_txt1->Clear();
}

void cMain::onEnterButtonClicked(wxCommandEvent& evt)
{
	std::string operation = std::string(m_txt1->GetValue().mb_str());

	// divide operation into first number, second number and the operand
	std::string number1 = "", oper = "", number2 = "";
	bool firstNumber = true;

	for (int i = 0; i < operation.length(); i++)
	{
		// build first number
		if (firstNumber && operation[i] != ' ' && std::isdigit(operation[i]) || operation[i] == '.')
		{
			number1 += operation[i];
		}

		// get operand
		else if (firstNumber && !std::isdigit(operation[i]) && operation[i] != ' ')
		{
			oper = operation[i];
			firstNumber = false;
		}

		// build second number
		else if (operation[i] != ' ' && std::isdigit(operation[i]) || operation[i] == '.')
		{
			number2 += operation[i];
		}
	}

	// output
	int answer = 0;
	
	// user input less than two numbers
	if (number1 == "" || number2 == "")
	{
		wxMessageBox("Error: Invalid operation");
		m_txt1->SetValue("");
	}

	else
	{
		int n1 = std::stoi(number1);
		int n2 = std::stoi(number2);

		// calculate answer
		std::string remain = "";

		switch (oper[0])
		{
		case '+':
			answer = n1 + n2;
			break;
		case '-':
			answer = n1 - n2;
			break;
		case '*':
			answer = n1 * n2;
			break;
		case '/':
			if (n2 == 0)
			{
				wxMessageBox("Error: Cannot divide by zero");
				answer = -1;
				break;
			}
			
			answer = n1 / n2;
			break;
		}
		std::string sAnswer = (answer == -1 ? "" : std::to_string(answer));

		m_txt1->SetValue(sAnswer);
	}
}