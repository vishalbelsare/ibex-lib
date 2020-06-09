#include "ibex_ExprDataAVM.h"
#include "ibex_ConvexEnvelope.h"

namespace ibex {

#define IMPL_FACT_0ARY(type, n) \
AVMData* ExprDataAVMFactory::init(const type& e) {\
    const Dim& d = e.dim; \
    return create_data(d.size(), n); \
}

#define IMPL_FACT_UNARY(type, n) \
AVMData* ExprDataAVMFactory::init(const type& e, AVMData&) {\
    const Dim& d = e.dim; \
    return create_data(d.size(), n); \
}

#define IMPL_FACT_BINARY(type, n) \
AVMData* ExprDataAVMFactory::init(const type& e, AVMData&, AVMData&) {\
    const Dim& d = e.dim; \
    return create_data(d.size(), n); \
}

#define GET_MAX_LIN(name) convex_envelope::MAX_LIN_##name

AVMData* ExprDataAVMFactory::init(const ExprSymbol& e) {
    int var_count = e.dim.size();
    AVMData* new_data = new AVMData;
    for(int i = 0; i < var_count; ++i) {
        new_data->x_indexes.emplace_back(current_var_index);
        current_var_index++;
    }
    /*new_data->begin_index = current_var_index;
    new_data->size = var_count;
    current_var_index += var_count;
    new_data->end_index = current_var_index;*/
    new_data->begin_lin_index = current_lin_index;
    new_data->lin_count = 0;
    new_data->end_lin_index = current_lin_index;
    return new_data;
}

AVMData* ExprDataAVMFactory::init(const ExprVector& e, Array<AVMData>& args_deco) {
    assert(e.dim.size() == args_deco.size());
    AVMData* new_data = new AVMData;
    for(int i = 0; i < args_deco.size(); ++i) {
        assert(args_deco[i].x_indexes.size() == 1);
        new_data->x_indexes.emplace_back(args_deco[i].x_indexes[0]);
    }
    new_data->begin_lin_index = current_lin_index;
    new_data->lin_count = 0;
    new_data->end_lin_index = current_lin_index;
    return new_data;
}

//AVMData* ExprDataAVMFactory::init(const ExprIndex& e, AVMData& data) {
    /*const DoubleIndex& index = e.index;
    const Dim& dim = e.dim;
    if(index.domain_ref()) {
        if(index.all()) {
            return &data;
        } else {
            switch(dim.type()) {
            case Dim::ROW_VECTOR:

            }
        }
    }*/
//}

IMPL_FACT_0ARY(ExprConstant, 0)

//IMPL_FACT_UNARY(ExprIndex, 0);
IMPL_FACT_UNARY(ExprMinus, GET_MAX_LIN(int_mul))
IMPL_FACT_UNARY(ExprSqr, GET_MAX_LIN(sqr))
IMPL_FACT_UNARY(ExprSqrt, GET_MAX_LIN(sqrt))
IMPL_FACT_UNARY(ExprExp, GET_MAX_LIN(exp))
IMPL_FACT_UNARY(ExprLog, GET_MAX_LIN(log))
IMPL_FACT_UNARY(ExprCos, GET_MAX_LIN(cos))
IMPL_FACT_UNARY(ExprAcos, GET_MAX_LIN(acos))
IMPL_FACT_UNARY(ExprSin, GET_MAX_LIN(sin))
IMPL_FACT_UNARY(ExprAsin, GET_MAX_LIN(asin))
IMPL_FACT_UNARY(ExprSinh, GET_MAX_LIN(cosh))
IMPL_FACT_UNARY(ExprAsinh, GET_MAX_LIN(asinh))
IMPL_FACT_UNARY(ExprCosh, GET_MAX_LIN(cosh))
IMPL_FACT_UNARY(ExprAcosh, GET_MAX_LIN(acosh))
IMPL_FACT_UNARY(ExprTan, GET_MAX_LIN(tan))
IMPL_FACT_UNARY(ExprAtan, GET_MAX_LIN(atan))
IMPL_FACT_UNARY(ExprTanh, GET_MAX_LIN(tanh))
IMPL_FACT_UNARY(ExprAtanh, GET_MAX_LIN(atanh))
IMPL_FACT_UNARY(ExprPower, GET_MAX_LIN(power))
IMPL_FACT_UNARY(ExprSign, GET_MAX_LIN(sign))
IMPL_FACT_UNARY(ExprAbs, GET_MAX_LIN(abs))
IMPL_FACT_BINARY(ExprAdd, GET_MAX_LIN(add))
IMPL_FACT_BINARY(ExprSub, GET_MAX_LIN(sub))
IMPL_FACT_BINARY(ExprMul, GET_MAX_LIN(mul))
IMPL_FACT_BINARY(ExprDiv, GET_MAX_LIN(div_from_mul))

#undef IMPL_FACT_UNARY
#undef IMPL_FACT_BINARY
#undef IMPL_FACT_0ARY

AVMData* ExprDataAVMFactory::create_data(int var_count, int lin_count) {
    AVMData* new_data = new AVMData;
    for(int i = 0; i < var_count; ++i) {
        new_data->x_indexes.emplace_back(current_index);
        current_index++;
    }
    /*new_data->begin_index = current_index;
    new_data->size = var_count;
    current_index += var_count;
    new_data->end_index = current_index;*/
    new_data->begin_lin_index = current_lin_index;
    new_data->lin_count = lin_count;
    current_lin_index += lin_count;
    new_data->end_lin_index = current_lin_index;
    return new_data;
}


ExprDataAVM::ExprDataAVM(const Function& f) : ExprData<AVMData>(f, create_avm_f(f.nb_var())) {
    nb_var = avm_f_->current_index;
    nb_lin = avm_f_->current_lin_index;
    delete avm_f_;
}

ExprDataAVMFactory& ExprDataAVM::create_avm_f(int nb_var) {
    avm_f_ = new ExprDataAVMFactory();
    avm_f_->current_index = nb_var;
    return *avm_f_;
}

} // namespace ibex
