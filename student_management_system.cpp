// Developed By Palash Gaikwad
#include <string>
#include <iostream>
#include <pqxx/pqxx>
#include <unistd.h>

int main()
{
  std::string input{};
  std::cout << "Developed By Palash Gaikwad \n\n\n";
  std::cout << "Welcome to Student Management System (DBMS)\n";

  do
  {
    // CONNECTION WITH POSTGRES DATABASE
    std::string connectionString = "host=localhost port=5432 dbname=school_database user=palash password =123";
    pqxx::connection connectionObject(connectionString.c_str());
    pqxx::work worker(connectionObject);

    // CODE FOR CHECKING PRE_EXISTING TABLES
    pqxx::result res = worker.exec(" SELECT table_name FROM information_schema.tables WHERE table_schema='public' AND table_type='BASE TABLE';");
    int counter{};
    std::string input2;

    if (res.size() > 0)
    {
      for (size_t i{}; i < res.size(); i++)
      {
        std::string b = to_string(res[i][0]);
        if (b == "datastudent")
        {
          counter++;
        }
      }
    }
    if (counter == 0)
    {
      worker.exec0("CREATE TABLE datastudent(id SERIAL PRIMARY KEY, student_name varchar NOT NULL, age integer, grade integer, division varchar(50), fees boolean);");
    }
    else if (res.size() == 0)
    {
      worker.exec0("CREATE TABLE datastudent(id SERIAL PRIMARY KEY, student_name varchar NOT NULL, age integer, grade integer, division varchar(50), fees boolean);");
    }

    // MAIN MENU AND ITS BRANCHES
    {
      std::cout << "MAIN MENU:\n\n";
      std::cout << "1.Add Student\n";
      std::cout << "2.Remove Student\n";
      std::cout << "3.Change Student Attributes / Modify Students \n";
      std::cout << "4.Output Student Data\n";
      std::cout << "Q.Quit\n \n";
      std::cin >> input;
    }

    // ADD STUDENT DATA INTO THE TABLE
    if (input == "1")
    {
      std::string student_name1{};
      int age{};
      int grade{};
      std::string division{};
      std::string fake_fees{};
      bool fees;
      std::cout << "Please input in the following order:\n Student age,\n Student Grade,\n Student Name,\n Student Division,\n Student Fees (Enter true if they have paid or enter false if they haven't)\n";
      std::cout << "Enter Age: ";
      std::cin >> age;
      std::cout << "Enter Grade: ";
      std::cin >> grade;
      std::cin.ignore();
      std::cout << "Enter Student Name: ";
      getline(std::cin, student_name1);
      std::cout << "Enter Division: ";
      getline(std::cin, division);
      std::cout << "Enter Fees: ";
      std::cin >> fake_fees;

      if (fake_fees == "True" || fake_fees == "true" || fake_fees == "TRUE")
      {
        fees = true;
      }
      else if (fake_fees == "False" || fake_fees == "false" || fake_fees == "FALSE")
      {
        fees = false;
      }
      else if (fake_fees != "True" || fake_fees != "true" || fake_fees != "false" || fake_fees != "False")
      {
        std::cout << "ERROR: Wrong input for fees (Only put True or False) \n";
      }

      // INSERT
      {
        try
        {
          worker.exec("INSERT INTO datastudent (id,student_name, age,grade,division,fees) VALUES (DEFAULT,'" + worker.esc(student_name1) + "' , '" + std::to_string(age) + "','" + std::to_string(grade) + "','" + worker.esc(division) + "','" + std::to_string(fees) + "');");
          worker.commit();
        }
        catch (const std::exception &e)
        {
          std::cerr << e.what() << std::endl;
        }
      }
    }

    // DELETE ROW FROM POSTGRES DATABASE
    else if (input == "2")
    {
      pqxx::result responser = worker.exec("SELECT * FROM datastudent");
      if (responser.size() == 0)
      {
        std::cout << " \n \n Nothing is inside of the table, Please make a student before deleting one \n \n";
      }
      else
      {
        int id_number{};
        std::cout << "Give id numbers for deleting: ";
        std::cout << "Enter Here: ";
        std::cin >> id_number;
        worker.exec("DELETE FROM datastudent WHERE id =" + std::to_string(id_number));
        worker.commit();
      }
    }

    // MODIFYING EXISTING DATA IN THE POSTGRES DATABASE
    else if (input == "3")
    {
      int id_number{};
      std::string change;
      std::string to_change_to{};
      int to_gang_to{};
      std::cout << "Enter id Number for student you wish to update: ";
      std::cout << "Enter Here: ";
      std::cin >> id_number;
      if (id_number == 0)
      {
        std::cout << "Invalid ID number, 0 is not a valid ID number. Please try again \n";
      }
      std::cout << "What column do you want to update: \n";
      std::cout << "age \n";
      std::cout << "grade \n";
      std::cout << "name \n";
      std::cout << "division \n";
      std::cout << "fees \n";
      std::cout << "Enter Here: ";
      std::cin >> change;
      std::cout << "\n\n********************************** \n";
      if (change == "name" || change == "division" || change == "Name" || change == "NAME" || change == "Division" || change == "DIVISION")
      {
        std::cout << "What do you want to change it to? \n";
        std::cout << "Enter Here: ";
        std::cin >> to_change_to;
        if (change == "name" || change == "Name" || change == "NAME")
        {
          worker.exec("UPDATE datastudent set student_name = '" + worker.esc(to_change_to) + "' WHERE id = " + std::to_string(id_number) + ";");
          worker.commit();
        }
        else
        {
          worker.exec("UPDATE datastudent set " + worker.esc(change) + " = '" + worker.esc(to_change_to) + "' WHERE id = " + std::to_string(id_number) + ";");
          worker.commit();
        }
      }
      else if (change == "age" || change == "Age" || change == "AGE" || change == "grade" || change == "Grade" || change == "GRADE")
      {
        std::cout << "What do you want to change it to? \n";
        std::cout << "Enter Here: ";
        std::cin >> to_gang_to;

        worker.exec("UPDATE datastudent set " + worker.esc(change) + " = " + std::to_string(to_gang_to) + " WHERE id = " + std::to_string(id_number) + ";");
        worker.commit();
      }
      else if (change == "fees" || change == "fee" || change == "Fees" || change == "Fee" || change == "FEES" || change == "FEE")
      {
        worker.exec("UPDATE datastudent set fees = false WHERE id = " + std::to_string(id_number) + ";");
        worker.commit();
        std::cout << "Changed fees to its opposite.";
      }
    }

    // OUTPUTS AND DIFFERENT DATA VIEW OPTIONS - POSTGRES DATABASE
    else if (input == "4")
    {
      std::cout << "\n\n";
      pqxx::result responser = worker.exec("SELECT * FROM datastudent");

      if (responser.size() == 0)
      {
        std::cout << "No data in table. Please add students before looking at the list.";
      }

      else if (responser.size() > 0)
      {
        int choice{};
        std::cout << "1.All Students\n";
        std::cout << "2.Grade wise (Ascending) \n";
        std::cout << "3.Grade and Division wise (Ascending) \n";
        std::cout << "4.Serial Number \n";
        std::cout << "Enter Here: ";
        std::cin >> choice;

        // SELECT * Command SQL
        if (choice == 1)
        {
          std::cout << "\n\n********************************** \n";

          for (int i{}; i < responser.size(); i++)
          {
            std::cout << responser[i][0] << " " << responser[i][1] << " " << responser[i][2] << " " << responser[i][3] << " " << responser[i][4] << " " << responser[i][5] << " " << responser[i][6] << "\n\n";
          }
          std::cout << "********************************** \n";
        }

        // SELECT COMMAND
        else if (choice == 2)
        {
          int grades{};
          std::cout << "Input grade to output in ascending: ";
          std::cout << "Enter Here: ";
          std::cin >> grades;
          responser = worker.exec("SELECT * from datastudent WHERE grade = '" + std::to_string(grades) + "' ORDER BY id ASC;");
          std::cout << "\n\n********************************** \n";

          for (int i{}; i < responser.size(); i++)
          {
            std::cout << responser[i][0] << " " << responser[i][1] << " " << responser[i][2] << " " << responser[i][3] << " " << responser[i][4] << " " << responser[i][5] << " " << responser[i][6] << "\n";
          }
          std::cout << "********************************** \n";
        }

        // SELECT COMMAND SPECIFIC RECORD
        else if (choice == 3)
        {
          int grades{};
          char divisions[2];
          std::cout << "Input grade and division: ";
          std::cout << "Enter Here: ";
          std::cin >> grades >> divisions[0];
          if (divisions[0] > 96 && divisions[0] < 123)
          {
            divisions[0] = divisions[0] - 32;
          }
          divisions[1] = '\0';
          responser = worker.exec("SELECT * FROM datastudent WHERE grade = '" + std::to_string(grades) + "' AND division = '" + worker.esc(divisions) + "'ORDER BY grade ASC,division ASC;");
          std::cout << "\n\n********************************** \n";
          for (int i{}; i < responser.size(); i++)
          {
            std::cout << responser[i][0] << " " << responser[i][1] << " " << responser[i][2] << " " << responser[i][3] << " " << responser[i][4] << " " << responser[i][5] << " " << responser[i][6] << "\n\n";
          }
          std::cout << "********************************** \n";
        }

        // SELECT SPECIFIC ROW
        else if (choice == 4)
        {
          int serials{};
          std::cout << "input serial number: ";
          std::cout << "Enter Here: ";
          std::cin >> serials;
          responser = worker.exec("SELECT * from datastudent where id = '" + std::to_string(serials) + "';");
          std::cout << "\n\n********************************** \n";
          for (int i{}; i < responser.size(); i++)
          {
            std::cout << responser[0][0] << " " << responser[0][1] << " " << responser[0][2] << " " << responser[0][3] << " " << responser[0][4] << " " << responser[0][5] << " " << responser[0][6] << "\n\n";
          }
          std::cout << "********************************** \n";
        }
      }
      std::cout << "\n";
    }
    sleep(1);
    std::cout << "Press Any Alphabet To Continue:";
    char hold;
    scanf("%c\n\n\n\n", &hold);
    std::cout << "\n\n";

  } while (input != "Q" && input != "q");

  std::cout << "Developed By Palash Gaikwad \n\n\n";
  return 0;
}