#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "boost/lexical_cast.hpp"

namespace chelp
{
	void hrule() { std::cout << "----------------------------------------" << std::endl; }

    bool confirmprompt(std::string prompt = "[y/n] ")
    {
        std::string input;

        while (true)
        {
            std::cout << prompt;
            std::getline(std::cin, input);

            if (input == "y" || input == "Y" || input == "yes" || input == "Yes" || input == "YES")
            {
                std::cout << std::endl;
                return true;
            }
            else if (input == "n" || input == "N" || input == "no" || input == "No" || input == "NO")
            {
                std::cout << std::endl;
                return false;
            }

            std::cout << "Invalid input.  Please respond with [y/n] selection." << std::endl;
        }
    }

	unsigned int picknumber(unsigned int numOfOptions, std::string prompt = "Pick an option: ")
	{
        if (numOfOptions == 0) throw std::invalid_argument("chelp::picknumber() - numOfOptions cannot be 0.");

        std::string input;
        unsigned int selection;

        while (true)
        {
            std::cout << prompt;
            std::getline(std::cin, input);

            try {
                selection = boost::lexical_cast<unsigned int>(input);
                if (selection >= numOfOptions)
                    std::cout << "Invalid selection.  ";
                else
                    break;
            }
            catch (boost::bad_lexical_cast) {
                std::cout << "Invalid input.  ";
            }
        }
        std::cout << std::endl;
        return selection;
	}
}