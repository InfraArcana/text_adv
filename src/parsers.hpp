#ifndef PARSERS_H
#define PARSERS_H

#include <vector>
#include <string>

class Cmd_Parser
{
public:
    Cmd_Parser() {}

    virtual ~Cmd_Parser() {}

    virtual std::vector<std::string> get_trigger_verbs() const = 0;

    virtual void handle_cmd(std::vector<std::string>& cmd) const = 0;
};

class Quit_Cmd_Parser : public Cmd_Parser
{
public:
    std::vector<std::string> get_trigger_verbs() const override;

    void handle_cmd(std::vector<std::string>& cmd) const override;
};

class Go_Cmd_Parser : public Cmd_Parser
{
public:
    std::vector<std::string> get_trigger_verbs() const override;

    void handle_cmd(std::vector<std::string>& cmd) const override;
};

class Look_Cmd_Parser : public Cmd_Parser
{
public:
    std::vector<std::string> get_trigger_verbs() const override;

    void handle_cmd(std::vector<std::string>& cmd) const override;
};

#endif // PARSERS_H
