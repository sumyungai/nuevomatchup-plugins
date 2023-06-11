#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "libcommon/lib/arguments.h"
#include "lib/range-array.h"
#include "lib/rqrmi-trainer.h"
#include "utils/common-utils.h"
#include "utils/string-operations.h"

/* Application arguments */
static arguments args[] = {
/* Name                 R  B  Def       Help */
{"file",                1, 0, NULL,     "Input ranges file."},
{"error",               0, 0, "64",     "(NuevoMatch Knob) Desireable error of "
                                        " models."},
{"max-sessions",        0, 0, "6",      "(NuevoMatch Knob) Maximum RQ-RMI "
                                        "retraining sessions."},
{"samples-per-session", 0, 0, "4000",   "(NuevoMatch Knob) Number of samples "
                                        "per submodel training session. "},
{"use-batching",        0, 1, NULL,     "(NuevoMatch Knob) Use batching "
                                        "when training."},
{"custom-rqrmi",        0, 0, "1,8,55", "(NuevoMatch Knob) use a custom size "
                                        "for the RQ-RMI models. Use number of"
                                        "submodels per stage, separated by"
                                        "comma. Example: '1,8,64'"},
{"compress",            0, 0, "1",      "(NuevoMatch Knob) use 'range-array' "
                                        "compression method to compress the "
                                        "ranges before (!) training."},
{"resolution",          0, 0, "21",      "(NuevoMatch Knob) resolution knob"},
{"submodel-chart",      0, 0, NULL,     "(Operation Knob) Generate data for "
                                        "submodel chart (expected vs. "
                                        "approximated)"},
{"floating-point",      0, 0, "org",     "quantizes the model using a custom format"},
{"print-map",           0, 0, NULL,     "prints a map of all the values used "},
{NULL,                  0, 0, NULL,     "Builds an RQRMI model from ranges "
                                        "textfile."},
};

#define T uint64_t
#define W 8

using rqrmi_train = rqrmi_trainer<W, T>;
using range_arr = range_array<T>;

static std::vector<T> ranges;
static std::vector<int> rqrmi_size;

static void
read_file()
{
    std::ifstream ifs;
    std::string line;
    range_arr rangearr;
    int compression;
    const char *filename;

    filename = ARG_STRING(args, "file", "");
    compression = ARG_INTEGER(args, "compress", 1);

    ifs.open(filename);
    if (ifs.eof()) {
        std::cout << "Error reading " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Reading ranges from \"" << filename << "\"..." << std::endl;
    while (std::getline(ifs, line)) {
        ranges.push_back(atol(line.c_str()));
    }
    std::cout << "Got " << ranges.size() << " ranges." << std::endl;

    if (compression == 1) {
        return;
    }

    std::cout << "Got custom compression " << compression << std::endl;
    rangearr.populate(ranges, compression, false);
    ranges.clear();
    ranges.reserve(rangearr.get_size());
    for (size_t i=0; i<rangearr.get_size(); ++i) {
        ranges.push_back(rangearr.get_values()[i]);
    }

    std::cout << "Got " << rangearr.get_size() << " ranges after compression."
              << std::endl;
}
std::ostream& operator<<
(std::ostream& out, const std::map <double, long int> &doubles)
{
    for(auto it = doubles.cbegin(); it != doubles.cend(); ++it)
    {
        out << it->first << " " <<it->second << "\n";
    }
    return out;
}

void print_map()
{
    const char *name = ARG_STRING(args, "print-map", NULL);
    cout<<name<<"\n";
    if (ARG_STRING(args, "print-map", NULL))
    {
    std::string n = name;
    std::ofstream mapout(n);
    mapout << std::fixed << std::setprecision(30) << Mydouble::getmap();
    std::cout << " * Exporting map " << name << " with " 
              << Mydouble::getmap().size() << " elements" << std::endl; 
    mapout.close();
    }
}

static void
train_model()
{
    rqrmi_train::model_type *model;
    rqrmi_train rqtrain;
    rqrmi_train::policy pol;
    StringOperations<int> sops;
    std::string value;

    pol.error_threshold = ARG_INTEGER(args, "error", 64);
    pol.fail_after_max_sessions = false;
    pol.max_retraining_sessions = ARG_INTEGER(args, "max-sessions", 6);
    pol.rqrmi_train_attrs.samples_per_session =
            ARG_INTEGER(args, "samples-per-session", 4e3);
    pol.rqrmi_train_attrs.use_batching = ARG_BOOL(args, "use-batching", 0);
    pol.rqrmi_train_attrs.hybrid_training = true;

    value = ARG_STRING(args, "custom-rqrmi", "1,8,55");
    rqrmi_size = sops.split(value, ",", StringOperations<uint32_t>::str2uint);

    std::cout << "Training RQRMI of size " << value << "..." << std::endl;
    rqtrain.set_policy(pol);
    rqtrain.build_database(ranges);
    rqtrain.on_new_message().add_listener(
    [&] (rqrmi_train &rq, const update_message& msg)
    {
        rqrmi_trainer_callback_print<W,T>(msg, stdout, 10);
    });

    model = rqtrain.train_rqrmi(rqrmi_size);
    const char *name = ARG_STRING(args, "submodel-chart", NULL);
    generate_submodel_chart<W,T>(*model, rqtrain, name, 1e3);
}
void set_resolution()
{
    int dig = ARG_INTEGER(args, "resolution", 20);
    Mydouble::set_resolution(dig);
}

void set_floating()
{
    const char *name = ARG_STRING(args, "floating-point", NULL);
    std::string n = name;
        
        if (n =="float16")
        {
        Mydouble::setexp(63);
        Mydouble::setman(0xFF800000000000);
        
        }
        else if (n == "float8_1")
        {
            Mydouble::setexp(63); 
            Mydouble::setman(0x80000000000000);
    
        }
        else if (n == "float8_2")
        {
            Mydouble::setexp(31);
            Mydouble::setman(0xC0000000000000);
            
        }
        else if (n == "half_prec")
        {
            Mydouble::setexp(31);
            Mydouble::setman(0xFFC0000000000);
            
        }
        else if (n == "Bfloat16")
        {
            Mydouble::setexp(255);
            Mydouble::setman(0xFE00000000000);
        }
        else if (n == "fp32")
        {
            Mydouble::setexp(255);
             Mydouble::setman(0xFFFFFE0000000);
        }
        
        
            
        

    }
    

int
main(int argc, char **argv)
{
    arg_parse(argc, argv, args);
    set_floating();
    set_resolution();
    read_file();
    train_model();
    print_map();
    
}
