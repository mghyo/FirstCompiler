
class Strategy;
class Program;

extern Program *program;
extern Strategy *strat;
extern std::stringstream *out;

extern void generate(Function *program, std::stringstream &out);
extern void emit(const std::string &op);
extern void emit(const std::string &op, const std::string &a1);
extern void emit(const std::string &op, const std::string &a1, const std::string &a2);
extern void emit(const std::string &op, const std::string &a1, const std::string &a2, const std::string &a3);
