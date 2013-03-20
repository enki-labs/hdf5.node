#include <node.h>
#include <string>

#include "H5Cpp.h"
#include "hdf5.h"

namespace NodeHDF5 {
    
    using namespace v8;
    
    DataSet::DataSet(H5::DataSet dataSet) {
        
        m_dataSet = dataSet;
        
    }
    
    Persistent<Function> DataSet::Constructor;
    
    void DataSet::Initialize () {        
        Local<FunctionTemplate> t = FunctionTemplate::New(New);
        
        // set properties
        t->SetClassName(String::New("DataSet"));
        t->InstanceTemplate()->SetInternalFieldCount(1);

        Constructor = Persistent<Function>::New(t->GetFunction());
    }
    
    Handle<Value> DataSet::New (const Arguments& args) {
        
        HandleScope scope;

        if (args.Length() != 2 || !args[0]->IsString() || !args[1]->IsObject()) {
            
            ThrowException(v8::Exception::SyntaxError(String::New("expected name, file")));
            return scope.Close(Undefined());
            
        }

        String::Utf8Value set_name (args[0]->ToString());
        File* file = ObjectWrap::Unwrap<File>(args[1]->ToObject());

        DataSet* dataSet = new DataSet(file->FileObject()->openDataSet(*set_name));
        dataSet->Wrap(args.This());
        
        // attach various properties
        args.This()->Set(String::NewSymbol("id"), Number::New(dataSet->m_dataSet.getId()));
        
        return args.This();
        
    }
    
    Handle<Value> DataSet::Instantiate (const char* name, Local<Object> file) {
        
        HandleScope scope;
        
        // group name and file reference
        Local<Value> argv[2] = {
                
                Local<Value>::New(String::New(name)),
                file
                
        };
        
        // return new group instance
        return scope.Close(Constructor->NewInstance(2, argv));
        
    }







};
