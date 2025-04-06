#include <stdio.h>     // fopen fprintf
#include <string.h>    // strerror
#include <errno.h>
#include <stdbool.h>   // needed for parse.h
#include <assert.h>
#include <stdint.h>

#include "print_protocol.h"
#include "parse.h"
#include "xtrace.h"          // getAtom

/*
struct constant {
    unsigned long value;
    const char *name;
};

struct request {
    const char *name;
    const struct parameter *parameters;
    const struct parameter *answers;

    request_func *request_func;
    reply_func *reply_func;
    // stack values to be transfered to the reply code
    int record_variables;
};
struct event {
    const char *name;
    const struct parameter *parameters;
    enum event_type { event_normal = 0, event_xge = 1} type;
#define event_COUNT 2
};

struct extension {
    const char *name;
    size_t namelen;
    const struct request *subrequests;
    unsigned char numsubrequests;
    const struct event *events;
    unsigned char numevents;
    const char * const *errors;
    unsigned char numerrors;
    unsigned short numxgevents;
    const struct event *xgevents;
};

struct parameter {
    // The offset within the event, request, reply or Struct this
    // applies to. If OFS_LATER it is after the last list item
    // in this parameter-list.
    size_t offse;
    // NULL means end of list
    const char *name;
    enum fieldtype {
        // signed endian specific:
        ft_INT8, ft_INT16, ft_INT32,
        // unsigned decimal endian specific:
        ft_UINT8, ft_UINT16, ft_UINT32,
        // unsigned hex endian specific:
        ft_CARD8, ft_CARD16, ft_CARD32,
        // enums (not in constant list is error):
        ft_ENUM8, ft_ENUM16, ft_ENUM32,
        // counts for following strings, lists, ...
        // value-mask for LISTofFormat
        ft_STORE8, ft_STORE16, ft_STORE32,
        // to be ft_GET later into the store register
        ft_PUSH8, ft_PUSH16, ft_PUSH32,
        // bitfields: multiple values are possible
        ft_BITMASK8, ft_BITMASK16, ft_BITMASK32,
        // Different forms of lists:
        //	- boring ones
        ft_STRING8, ft_LISTofCARD32, ft_LISTofATOM,
        ft_LISTofCARD8, ft_LISTofCARD16,
        ft_LISTofUINT8, ft_LISTofUINT16,
        ft_LISTofUINT32,
        ft_LISTofINT8, ft_LISTofINT16,
        ft_LISTofINT32,
        //	- one of the above depening on last FORMAT
        ft_LISTofFormat,
        //	- iterate of list description in constants field
        ft_LISTofStruct,
        //	- same but length is mininum length and
        //	  actual length is taken from end of last list
        //	  or LASTMARKER, unless there is a SIZESET
        ft_LISTofVarStruct,
        //	- like ENUM for last STORE, but constants
        //	  are of type (struct value*) interpreteted at this
        //	  offset
        ft_LISTofVALUE,
        // an LISTofStruct with count = 1
        ft_Struct,
        // specify bits per item for LISTofFormat
        ft_FORMAT8,
        // an event
        // (would have also been possible with Struct and many IF)
        ft_EVENT,
        // jump to other parameter list if matches
        ft_IF8,
        ft_IF16,
        ft_IF32,
        // jump to other parameter list if matches atom name
        ft_IFATOM,
        // set end of last list manually, (for LISTofVarStruct)
        ft_LASTMARKER,
        // set the end of the current context, also change length
        // of a VarStruct:
        ft_SET_SIZE,
        // a ft_CARD32 looking into the ATOM list
        ft_ATOM,
        // always big endian
        ft_BE32,
        // get the #ofs value from the stack. (0 is the last pushed)
        ft_GET,
        // a fixed-point number 16+16 bit
        ft_FIXED,
        // a list of those
        ft_LISTofFIXED,
        // a fixed-point number 32+32 bit
        ft_FIXED3232,
        // a list of those
        ft_LISTofFIXED3232,
        // a 32 bit floating pointer number
        ft_FLOAT32,
        // a list of those
        ft_LISTofFLOAT32,
        // fraction with nominator and denominator 16 bit
        ft_FRACTION16_16,
        // dito 32 bit
        ft_FRACTION32_32,
        // nominator is unsigned
        ft_UFRACTION32_32,
        // a 64 bit number consisting of first the high 32 bit, then
        // the low 32 bit
        ft_INT32_32,
        // decrement stored value by specific value
        ft_DECREMENT_STORED,
        ft_DIVIDE_STORED,
        // set stored value to specific value
        ft_SET
    } type;
    union parameter_option {
        // for integers and fields of integers
        const struct constant *constants;
        // for IFs, Structs, ...
        const struct parameter *parameters;
        // for LISTofVALUE
        const struct value *values;
    } o;
};
struct value {
    unsigned long flag;
    // NULL means EndOfValues
    const char *name;
    // only elementary type (<= ft_BITMASK32 are allowed ),
    enum fieldtype type;
    const struct constant *constants;
};

extern const struct request *requests;
extern size_t num_requests;
extern const struct event *events;
extern size_t num_events;
extern const char * const *errors;
extern size_t num_errors;
extern const struct extension *extensions;
extern size_t num_extensions;
extern const struct parameter *unexpected_reply;
extern const struct parameter *setup_parameters;
*/

// TBD also defined in parse.c
#define OFS_LATER ((size_t)-1)

#define DUMP_FILENAME "x11_protocol.txt"
#define TAB2 "  "
#define TAB4 "    "
#define TABx1 TAB4
#define TABx2 TABx1 TABx1
#define TABx3 TABx2 TABx1
#define TABx4 TABx3 TABx1
#define TABx5 TABx4 TABx1
#define TABx6 TABx5 TABx1
#define TABx7 TABx6 TABx1
#define TABx8 TABx7 TABx1
#define TABx9 TABx8 TABx1
#define TABx10 TABx9 TABx1
/* #define TABx11 TABx10 TABx1 */
/* #define TABx12 TABx11 TABx1 */
/* #define TABx13 TABx12 TABx1 */
/* #define TABx14 TABx13 TABx1 */
/* #define TABx15 TABx14 TABx1 */
/* #define TABx16 TABx15 TABx1 */
/* #define TABx17 TABx16 TABx1 */
/* #define TABx18 TABx17 TABx1 */
/* #define TABx19 TABx18 TABx1 */
/* #define TABx20 TABx19 TABx1 */
/* #define TABx21 TABx20 TABx1 */
/* #define TABx22 TABx21 TABx1 */
/* #define TABx23 TABx22 TABx1 */
/* #define TABx24 TABx23 TABx1 */
/* #define TABx25 TABx24 TABx1 */

/* unsigned int max_tab_ct = 0; */

static const char* base_indent(const unsigned int base_tab_ct) {
    assert( base_tab_ct <= 10 );
    /* if ( base_tab_ct > max_tab_ct ) */
    /*     max_tab_ct = base_tab_ct; */
    switch ( base_tab_ct ) {
    case 1: return TABx1;
    case 2: return TABx2;
    case 3: return TABx3;
    case 4: return TABx4;
    case 5: return TABx5;
    case 6: return TABx6;
    case 7: return TABx7;
    case 8: return TABx8;
    case 9: return TABx9;
    case 10: return TABx10;
    /* case 11: return TABx11; */
    /* case 12: return TABx12; */
    /* case 13: return TABx13; */
    /* case 14: return TABx14; */
    /* case 15: return TABx15; */
    /* case 16: return TABx16; */
    /* case 17: return TABx17; */
    /* case 18: return TABx18; */
    /* case 19: return TABx19; */
    /* case 20: return TABx20; */
    /* case 21: return TABx21; */
    /* case 22: return TABx22; */
    /* case 23: return TABx23; */
    /* case 24: return TABx24; */
    /* case 25: return TABx25; */
    }
    return "";
}

static unsigned int field_width(unsigned int max) {
    unsigned int pow10 = 0;
    for (; max; max /= 10) {
        ++pow10;
    }
    return pow10;
}

static const char* request_func_name(request_func* rf) {
    if (rf == requestQueryExtension ) {
        return "requestQueryExtension";
    } else if (rf == requestInternAtom ) {
        return "requestInternAtom";
    } else if (rf == requestGetAtomName ) {
        return "requestGetAtomName";
    } else if ( rf == NULL ) {
        return NULL;
    }
    return "(unknown)";
}

static const char* reply_func_name(reply_func* rf) {
    if (rf == replyListFontsWithInfo ) {
        return "replyListFontsWithInfo";
    } else if (rf == replyQueryExtension ) {
        return "replyQueryExtension";
    } else if (rf == replyInternAtom ) {
        return "replyInternAtom";
    } else if (rf == replyGetAtomName ) {
        return "replyGetAtomName";
    } else if ( rf == NULL ) {
        return NULL;
    }
    return "(unknown)";
}

static const char* fieldtype_name(const enum fieldtype ft) {
    assert(ft_INT8 == 0);
    assert(ft >= ft_INT8 && ft <= ft_SET);
    static const char* fieldtype_names[] = {
        // signed endian specific:
        "ft_INT8", "ft_INT16", "ft_INT32",
        // unsigned decimal endian specific:
        "ft_UINT8", "ft_UINT16", "ft_UINT32",
        // unsigned hex endian specific:
        "ft_CARD8", "ft_CARD16", "ft_CARD32",
        // enums (not in constant list is error):
        "ft_ENUM8", "ft_ENUM16", "ft_ENUM32",
        // counts for following strings, lists, ...
        // value-mask for LISTofFormat
        "ft_STORE8", "ft_STORE16", "ft_STORE32",
        // to be "ft_GET" later into the store register
        "ft_PUSH8", "ft_PUSH16", "ft_PUSH32",
        // bitfields: multiple values are possible
        "ft_BITMASK8", "ft_BITMASK16", "ft_BITMASK32",
        // Different forms of lists:
        //	- boring ones
        "ft_STRING8", "ft_LISTofCARD32", "ft_LISTofATOM",
        "ft_LISTofCARD8", "ft_LISTofCARD16",
        "ft_LISTofUINT8", "ft_LISTofUINT16",
        "ft_LISTofUINT32",
        "ft_LISTofINT8", "ft_LISTofINT16",
        "ft_LISTofINT32",
        //	- one of the above depening on last FORMAT
        "ft_LISTofFormat",
        //	- iterate of list description in constants field
        "ft_LISTofStruct",
        //	- same but length is mininum length and
        //	  actual length is taken from end of last list
        //	  or LASTMARKER, unless there is a SIZESET
        "ft_LISTofVarStruct",
        //	- like ENUM for last STORE, but constants
        //	  are of type (struct value*) interpreteted at this
        //	  offset
        "ft_LISTofVALUE",
        // an LISTofStruct with count = 1
        "ft_Struct",
        // specify bits per item for LISTofFormat
        "ft_FORMAT8",
        // an event
        // (would have also been possible with Struct and many IF)
        "ft_EVENT",
        // jump to other parameter list if matches
        "ft_IF8",
        "ft_IF16",
        "ft_IF32",
        // jump to other parameter list if matches atom name
        "ft_IFATOM",
        // set end of last list manually, (for LISTofVarStruct)
        "ft_LASTMARKER",
        // set the end of the current context, also change length
        // of a VarStruct:
        "ft_SET_SIZE",
        // a "ft_CARD32" looking into the ATOM list
        "ft_ATOM",
        // always big endian
        "ft_BE32",
        // get the #ofs value from the stack. (0 is the last pushed)
        "ft_GET",
        // a fixed-point number 16+16 bit
        "ft_FIXED",
        // a list of those
        "ft_LISTofFIXED",
        // a fixed-point number 32+32 bit
        "ft_FIXED3232",
        // a list of those
        "ft_LISTofFIXED3232",
        // a 32 bit floating pointer number
        "ft_FLOAT32",
        // a list of those
        "ft_LISTofFLOAT32",
        // fraction with nominator and denominator 16 bit
        "ft_FRACTION16_16",
        // ditto 32 bit
        "ft_FRACTION32_32",
        // nominator is unsigned
        "ft_UFRACTION32_32",
        // a 64 bit number consisting of first the high 32 bit, then
        // the low 32 bit
        "ft_INT32_32",
        // decrement stored value by specific value
        "ft_DECREMENT_STORED",
        "ft_DIVIDE_STORED",
        // set stored value to specific value
        "ft_SET"
    };
    return fieldtype_names[ft];
}

static void fprint_constants(FILE* ofs, const unsigned int base_tab_ct,
                             const struct constant* constants,
                             const char* constants_varname,
                             const enum fieldtype type) {
    assert(ofs != NULL);
    assert(constants_varname != NULL);
    const char* _base_indent = base_indent(base_tab_ct);
    if (constants == NULL) {
        fprintf( ofs, "%s%s: (null)\n",
                 _base_indent, constants_varname );
        return;
    } else {
        fprintf( ofs, "%s%s: {\n",
                 _base_indent, constants_varname );
    }
//    assert( type <= ft_BITMASK32 );
    enum fieldtype _type;
    switch (type) {
    case ft_LISTofATOM:   _type = ft_ATOM;   break;
    case ft_LISTofCARD8:  _type = ft_CARD8;  break;
    case ft_LISTofCARD16: _type = ft_CARD16; break;
    case ft_LISTofCARD32: _type = ft_CARD32; break;
    case ft_LISTofUINT8:  _type = ft_UINT8;  break;
    case ft_LISTofUINT16: _type = ft_UINT16; break;
    case ft_LISTofUINT32: _type = ft_UINT32; break;
    case ft_LISTofINT8:   _type = ft_INT8;   break;
    case ft_LISTofINT16:  _type = ft_INT16;  break;
    case ft_LISTofINT32:  _type = ft_INT32;  break;
    default:              _type = type;      break;
    }
    for (const struct constant* _const = constants;
         _const != NULL && _const->name != NULL; ++_const) {
        fprintf( ofs, TABx1 "%s{\n",
                 _base_indent );
        // Note that endianness normally matters when parsing x11 packets,
        //   as established by the first byte in the initial client packet.
        //   However we are only printing xtrace's model of the protocol, and
        //   not actual packets, so only need to use the default byte order.
        switch (_type) {
        case ft_INT8:
        case ft_INT16:
        case ft_INT32:
            fprintf( ofs, TABx2 "%svalue: %li\n",
                     _base_indent, _const->value );
            break;
        case ft_UINT8:
        case ft_UINT16:
        case ft_UINT32:
            fprintf( ofs, TABx2 "%svalue: %lu\n",
                     _base_indent, _const->value );
            break;
        case ft_CARD8:
        case ft_CARD16:
        case ft_CARD32:
            fprintf( ofs, TABx2 "%svalue: %lu\n",
                     _base_indent, _const->value );
            break;
        case ft_ENUM8:
        case ft_ENUM16:
        case ft_ENUM32:
            fprintf( ofs, TABx2 "%svalue: %li\n",
                     _base_indent, _const->value );
            break;
        case ft_STORE8:
        case ft_STORE16:
        case ft_STORE32:
        case ft_PUSH8:
        case ft_PUSH16:
        case ft_PUSH32:
            fprintf( ofs, TABx2 "%svalue: %lu\n",
                     _base_indent, _const->value );
            break;
        case ft_BITMASK8:
        case ft_BITMASK16:
        case ft_BITMASK32:
            fprintf( ofs, TABx2 "%svalue: 0x%lx\n",
                     _base_indent, _const->value );
            break;
        case ft_ATOM:
            const char* atom = getAtom( NULL, _const->value );
            if ( atom == NULL )
                fprintf( ofs, TABx2 "%svalue: %lu ([unrecognized atom])\n",
                         _base_indent, _const->value );
            else
                fprintf( ofs, TABx2 "%svalue: %lu (\"%s\")\n",
                         _base_indent, _const->value, atom );
            break;
        default:
            // TBD detect any other values above ft_BITMASK32
            puts( fieldtype_name(type) );
            assert( 0 );
            break;
        }
        fprintf( ofs, TABx2 "%sname: %s\n",
                 _base_indent, _const->name );
        fprintf( ofs, TABx1 "%s}\n",
                 _base_indent );
    }
    fprintf( ofs, "%s}\n",
             _base_indent );
}

static void fprint_values(FILE* ofs, const unsigned int base_tab_ct,
                          const struct value* values,
                          const char* values_varname) {
    assert(ofs != NULL);
    assert(values_varname != NULL);
    const char* _base_indent = base_indent(base_tab_ct);
    if (values == NULL) {
        fprintf( ofs, "%s%s: (null)\n",
                 _base_indent, values_varname );
        return;
    } else {
        fprintf( ofs, "%s%s: {\n",
                 _base_indent, values_varname );
    }
    for ( const struct value* val = values;
          val != NULL && val->name != NULL; ++val ) {
        fprintf( ofs, TABx1 "%s{\n",
                 _base_indent );
        fprintf( ofs, TABx2 "%sflag: 0x%lx\n",
                 _base_indent, val->flag );
        fprintf( ofs, TABx2 "%sname: %s\n",
                 _base_indent, val->name );
        fprintf( ofs, TABx2 "%stype: %s\n",
                 _base_indent, fieldtype_name(val->type) );
        fprint_constants( ofs, base_tab_ct + 2,
                          val->constants, "constants", val->type);
        fprintf( ofs, TABx1 "%s}\n",
                 _base_indent );
    }
    fprintf( ofs, "%s}\n",
             _base_indent );
}

static void fprint_parameters(FILE* ofs, const unsigned int base_tab_ct,
                              const struct parameter* parameters,
                              const char* parameters_varname) {
    assert(ofs != NULL);
    assert(parameters_varname != NULL);
    const char* _base_indent = base_indent(base_tab_ct);
    if (parameters == NULL) {
        fprintf( ofs, "%s%s: (null)\n",
                 _base_indent, parameters_varname );
        return;
    } else {
        fprintf( ofs, "%s%s: {\n",
                 _base_indent, parameters_varname );
    }
    for ( const struct parameter* param = parameters;
          param != NULL && param->name != NULL; ++param ) {
        fprintf( ofs, TABx1 "%s{\n",
                 _base_indent );
        if ( param->offse == OFS_LATER ) {
            fprintf( ofs, TABx2 "%soffse: OFS_LATER ((size_t)-1)\n",
                     _base_indent, param->offse );
        } else {
            fprintf( ofs, TABx2 "%soffse: %lu\n",
                     _base_indent, param->offse );
        }
        unsigned char* name_arr = (unsigned char*)(param->name);
        switch (param->type) {
        case ft_IF8:                // used, non-null parameter_option
            fprintf( ofs, TABx2 "%sname: [ %3u ] (encoded: %u)\n",
                     _base_indent, name_arr[0], name_arr[0] );
            assert( name_arr[1] == 0 );
            break;
        case ft_IF16:
            fprintf( ofs, TABx2 "%sname: [ %u %u ] (encoded: %u)\n",
                     _base_indent, name_arr[0], name_arr[1],
                     // comparison value from parse.c:print_parameters
                     (unsigned int)name_arr[1] +
                     (unsigned int)name_arr[0] << 8 );
            assert( name_arr[2] == 0 );
            break;
        case ft_IF32:               // used, non-null parameter_option
            fprintf( ofs, TABx2 "%sname: [ %u %u %u %u ] (encoded: %lu)\n",
                     _base_indent, name_arr[0], name_arr[1],
                     name_arr[2], name_arr[3],
                     // comparison value from parse.c:print_parameters
                     (unsigned long)name_arr[3] +
                     (unsigned long)name_arr[2] << 8 +
                     (unsigned long)name_arr[1] << 16 +
                     (unsigned long)name_arr[0] << 24 );
            assert( name_arr[4] == 0 );
            break;
        case ft_IFATOM:             // used, non-null parameter_option
        default:
            fprintf( ofs, TABx2 "%sname: %s\n",
                     _base_indent, param->name );
            break;
        }
        fprintf( ofs, TABx2 "%stype: %s\n",
                 _base_indent, fieldtype_name(param->type) );
        // intentionally redundant check of union
        if ( param->o.constants != NULL || param->o.parameters != NULL ||
             param->o.values != NULL ) {
            switch (param->type) {
            case ft_INT8:               // used
            case ft_INT16:              // used
            case ft_INT32:              // used
            case ft_UINT8:              // used, non-null parameter_option
            case ft_UINT16:             // used, non-null parameter_option
            case ft_UINT32:             // used
            case ft_CARD8:              // used, non-null parameter_option
            case ft_CARD16:             // used, non-null parameter_option
            case ft_CARD32:             // used, non-null parameter_option
            case ft_ENUM8:              // used, non-null parameter_option
            case ft_ENUM16:             // used, non-null parameter_option
            case ft_ENUM32:             // used, non-null parameter_option
                // Stores a value in a single variable (stored in server)
            case ft_STORE8:             // used
            case ft_STORE16:            // used
            case ft_STORE32:            // used
                // Pushes values to a stack (stored in server)
            case ft_PUSH8:              // used
            case ft_PUSH16:
            case ft_PUSH32:             // used
            case ft_BITMASK8:           // used, non-null parameter_option
            case ft_BITMASK16:          // used, non-null parameter_option
            case ft_BITMASK32:          // used, non-null parameter_option
                fprint_constants(ofs, base_tab_ct + 2,
                                 param->o.constants, "o.constants", param->type);
                break;
            case ft_STRING8:            // used
                break;
            case ft_LISTofCARD32:       // used, non-null parameter_option
            case ft_LISTofATOM:         // used
            case ft_LISTofCARD8:        // used
            case ft_LISTofCARD16:       // used
            case ft_LISTofUINT8:
            case ft_LISTofUINT16:       // used
            case ft_LISTofUINT32:       // used
            case ft_LISTofINT8:
            case ft_LISTofINT16:
            case ft_LISTofINT32:
                fprint_constants(ofs, base_tab_ct + 2,
                                 param->o.constants, "o.constants", param->type);
                break;
            case ft_LISTofFormat:       // used
                break;
            case ft_LISTofStruct:       // used, non-null parameter_option
            {
                const struct parameter* parameters = param->o.parameters;
                assert( parameters != NULL && parameters->name == NULL &&
                        parameters->offse > 0 );
                fprintf( ofs, TABx2 "%so.parameters->offse (struct size in bytes): %lu\n",
                         _base_indent, parameters->offse );
                fprint_parameters(ofs, base_tab_ct + 2, parameters + 1,
                                  "o.parameters + 1 (struct members)" );
            }
                break;
            case ft_LISTofVarStruct:    // used, non-null parameter_option
            {
                const struct parameter* parameters = param->o.parameters;
                assert( parameters != NULL && parameters->name == NULL &&
                        parameters->offse > 0 );
                fprintf( ofs, TABx2 "%so.parameters->offse (_minimum_ struct size in bytes): %lu\n",
                         _base_indent, parameters->offse );
                fprint_parameters(ofs, base_tab_ct + 2, parameters + 1,
                                  "o.parameters + 1 (struct members)" );
            }
                break;
            case ft_LISTofVALUE:        // used, non-null parameter_option
                fprint_values(ofs, base_tab_ct + 2,
                              param->o.values, "o.values");
                break;
            case ft_Struct:             // used, non-null parameter_option
                // LISTofStruct of length 1
            {
                const struct parameter* parameters = param->o.parameters;
                assert( parameters != NULL && parameters->name == NULL &&
                        parameters->offse > 0 );
                fprintf( ofs, TABx2 "%so.parameters->offse (struct size in bytes): %lu\n",
                         _base_indent, parameters->offse );
                fprint_parameters(ofs, base_tab_ct + 2, parameters + 1,
                                  "o.parameters + 1 (struct members)" );
            }
                break;
            case ft_FORMAT8:            // used
            case ft_EVENT:              // used
                break;
            case ft_IF8:                // used, non-null parameter_option
            case ft_IF16:
            case ft_IF32:               // used, non-null parameter_option
            case ft_IFATOM:             // used, non-null parameter_option
                fprint_parameters(ofs, base_tab_ct + 2,
                                  param->o.parameters, "o.parameters" );
                break;
                // TBD what is the significance LASTMARKER when we can detect
                //   the end of a series of constants/parameters/values by
                //   NULL or NULL name?
            case ft_LASTMARKER:         // used
            case ft_SET_SIZE:
                break;
                // TBD can be considered constant (ft_CARD32 at least in xtrace
                //   impl,) as it is essentially a 1-based index into dedicated
                //   string storage (static for ATOMs 1-68 predefined by the
                //   protocol, and heap allocated for all others) see getAtom and:
                //   - https://www.x.org/releases/current/doc/xproto/x11protocol.pdf
            case ft_ATOM:               // used, non-null parameter_option
                const char* atom = getAtom( NULL, param->o.constants->value );
                if ( atom == NULL )
                    fprintf( ofs, TABx2 "%so.constants->value: %lu ([unrecognized atom])\n",
                             _base_indent, param->o.constants->value );
                else
                    fprintf( ofs, TABx2 "%so.constants->value: %lu (\"%s\")\n",
                             _base_indent, param->o.constants->value, atom );
                break;
            case ft_BE32:
            case ft_GET:                // used
            case ft_FIXED:              // used
            case ft_LISTofFIXED:        // used
            case ft_FIXED3232:
            case ft_LISTofFIXED3232:    // used
            case ft_FLOAT32:
            case ft_LISTofFLOAT32:
            case ft_FRACTION16_16:      // used
            case ft_FRACTION32_32:
            case ft_UFRACTION32_32:
            case ft_INT32_32:           // used
            case ft_DECREMENT_STORED:   // used
            case ft_DIVIDE_STORED:
            case ft_SET:                // used
                break;
            }
        }
        fprintf( ofs, TABx1 "%s}\n",
                 _base_indent );
    }
    fprintf( ofs, "%s}\n",
             _base_indent );
}

static void fprint_requests(FILE* ofs, const unsigned int base_tab_ct,
                            const struct request* requests,
                            const unsigned int request_ct,
                            const char* request_ct_varname) {
    assert(ofs != NULL);
    assert(requests != NULL);
    assert(request_ct_varname != NULL);
    const char* _base_indent = base_indent(base_tab_ct);
    fprintf( ofs, "%s%s: %u\n",
             _base_indent, request_ct_varname, request_ct );
    for (unsigned int i = 0; i < request_ct; ++i) {
        const struct request req = requests[i];
        fprintf( ofs, "%srequests[%u] {\n",
                 _base_indent, i );
        fprintf( ofs, TABx1 "%sname: %s\n",
                 _base_indent, req.name );
        fprint_parameters( ofs, base_tab_ct + 1,
                           req.parameters, "parameters" );
        fprint_parameters( ofs, base_tab_ct + 1,
                           req.answers, "answers" );
        fprintf( ofs, TABx1 "%srequest_func: %s\n",
                 _base_indent, request_func_name(req.request_func) );
        fprintf( ofs, TABx1 "%sreply_func: %s\n",
                 _base_indent, reply_func_name(req.reply_func) );
        fprintf( ofs, TABx1 "%s// stack values to be transferred to the reply code\n",
                 _base_indent );
        fprintf( ofs, TABx1 "%srecord_variables: %i\n",
                 _base_indent, req.record_variables );
        fprintf( ofs, "%s}\n",
                 _base_indent );
    }
}

static void fprint_events(FILE* ofs, const unsigned int base_tab_ct,
                          const struct event* events,
                          const unsigned int event_ct,
                          const char* event_ct_varname) {
    assert(ofs != NULL);
    assert(events != NULL);
    assert(event_ct_varname != NULL);
    const char* _base_indent = base_indent(base_tab_ct);
    fprintf( ofs, "%s%s: %u\n",
             _base_indent, event_ct_varname, event_ct );
    if (event_ct == 0)
        return;
    for (unsigned int i = 0; i < event_ct; ++i) {
        const struct event evnt = events[i];
        fprintf( ofs, "%sevents[%u] {\n",
                 _base_indent, i );
        fprintf( ofs, TABx1 "%sname: %s\n",
                 _base_indent, evnt.name );
        fprint_parameters( ofs, base_tab_ct + 1,
                           evnt.parameters, "parameters" );
        fprintf( ofs, TABx1 "%stype: %s (%i)\n",
                 _base_indent,
                 evnt.type == 0 ? "event_normal" : "event_xge", evnt.type );
        fprintf( ofs, "%s}\n",
                 _base_indent );
    }
}

static void fprint_errors(FILE* ofs, const unsigned int base_tab_ct,
                          const char* const* errors,
                          const unsigned int error_ct,
                          const char* error_ct_varname) {
    assert(ofs != NULL);
    assert(errors != NULL);
    assert(error_ct_varname != NULL);
    const char* _base_indent = base_indent(base_tab_ct);
    fprintf( ofs, "%s%s: %u\n",
             _base_indent, error_ct_varname, error_ct );
    for (unsigned int i = 0, iwidth = field_width(error_ct);
         i < error_ct; ++i) {
        const char* error = errors[i];
        fprintf( ofs, "%serrors[%*u] %s\n",
                 _base_indent, (int)iwidth, i, error );
    }
}

static void fprint_extensions(FILE* ofs) {
    fprintf( ofs, "num_extensions: %lu\n", num_extensions );
    for (size_t i = 0; i < num_extensions; ++i) {
        const struct extension ext = extensions[i];
        fprintf( ofs, "extensions[%lu] {\n", i );
        fprintf( ofs, TABx1 "name: %s\n", ext.name );
        fprintf( ofs, TABx1 "namelen: %lu\n", ext.namelen );
        fprint_requests( ofs, 1, ext.subrequests, ext.numsubrequests, "numsubrequests" );
        fprint_events( ofs, 1, ext.events, ext.numevents, "numevents" );
        fprint_errors( ofs, 1, ext.errors, ext.numerrors, "numerrors" );
        fprint_events( ofs, 1, ext.xgevents, ext.numxgevents, "numxgevents" );
        fprintf( ofs, "}\n" );
    }
}

void print_protocol(void) {
    FILE *ofs = fopen(DUMP_FILENAME, "w");
    if ( ofs == NULL ) {
        fprintf( stderr,
                 "%s fopen(\"%s\", \"%s\"): %d=%s\n",
                 __func__, DUMP_FILENAME, "w", errno, strerror(errno) );
        return;
    }

    fprintf( ofs, "parse state after `finalize_everything(parser)`:\n");
    fprintf( ofs, "\n");

    /* extern const struct request *requests; */
    /* extern size_t num_requests; */
    fprint_requests( ofs, 0, requests, num_requests, "num_requests" );
    fprintf( ofs, "\n\n" );

    /* extern const struct event *events; */
    /* extern size_t num_events; */
    fprint_events( ofs, 0, events, num_events, "num_events" );
    fprintf( ofs, "\n\n" );

    /* extern const char* const* errors; */
    /* extern size_t num_errors; */
    fprint_errors( ofs, 0, errors, num_errors, "num_errors" );
    fprintf( ofs, "\n\n" );

    /* extern const struct extension *extensions; */
    /* extern size_t num_extensions; */
    fprint_extensions( ofs );
    fprintf( ofs, "\n\n" );

    /* extern const struct parameter *unexpected_reply; */
    fprint_parameters( ofs, 0, unexpected_reply, "unexpected_reply" );
    fprintf( ofs, "\n\n" );

    /* extern const struct parameter *setup_parameters; */
    fprint_parameters( ofs, 0, setup_parameters, "setup_parameters" );

    fclose( ofs );
    /* printf("max_tab_ct: %u\n", max_tab_ct); */
    return;
}
