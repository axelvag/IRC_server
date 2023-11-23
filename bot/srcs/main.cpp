# include "../includes/bot.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Error: Usage /bot...." << std::endl;
        return 1;
    }


    Bot bot = Bot("Bot", std::atoi(av[1]), av[2], IP);

    if (bot.Bot_Start() == 1)
        return 1;

    if (bot.Bot_Stop() == 1)
        return 1;

    return 0;
}