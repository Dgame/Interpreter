#include "Interpreter.hpp"
#include "Variable.hpp"

void Interpreter::makeVar(bool constant) {
    this->cursor.expect(Token::Identifier);

    Variable var(this->cursor.token.id, constant);

    this->cursor.expect(Token::Assign);
    this->cursor.getToken();

    var.val = this->cursor.token;

    this->vars[var.name] = var;

    this->cursor.expect(Token::StmtEnd);
}

void Interpreter::interpret(const std::string& content) {
    this->cursor = Cursor(&content[0]);

    Token::Type tok = this->cursor.getToken();
    while (tok != Token::Eof) {
        switch (tok) {
            case Token::Constant:
                this->makeVar(true);
                break;
            case Token::Variable:
                this->makeVar(false);
                break;
            case Token::Identifier:
            {
                const std::string id = this->cursor.token.id;

                tok = this->cursor.getToken();
                if (tok == Token::Assign) {
                    this->cursor.getToken();

                    const Value val = this->cursor.token;
                    auto it = this->vars.find(id);

                    if (it == this->vars.end()) {
                        error("No such Variable ", id);
                        throw "Interpreter Error";
                    } else if (it->second.isConst) {
                        error("Cannot modify const Variable ", id);
                        throw "Interpreter Error";
                    } else {
                        it->second.val = val;
                    }
                }

                this->cursor.expect(Token::StmtEnd);
            }
            break;
            case Token::Output:
            {
                tok = this->cursor.getToken();

                u32_t i = 0;
                while (tok != Token::StmtEnd && tok != Token::Eof && i < 255) {
                    switch (tok) {
                        case Token::Identifier:
                        {
                            const std::string id = this->cursor.token.id;
                            if (this->vars.find(id) != this->vars.end()) {
                                if (i > 0) {
                                    std::cout << ' ';
                                }

                                std::cout << this->vars[id];
                            }
                        }
                        break;
                    }

                    tok = this->cursor.getToken();
                    i++;
                }

                std::cout << std::endl;
            }
            break;
        }

        tok = this->cursor.getToken();
    }
}