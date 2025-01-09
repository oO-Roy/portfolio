/*  File: cpp2c - c part.
    Version 1.3.
    Status: APPROVED */

#include <stdio.h> /*printf, size_t*/
#include <stdlib.h> /*malloc, free*/

#define ERROR -1

#define UNUSED(x) (void)(x)
#define TEMPLATE_MAX(T)         \
T MAX_##T(T a, T b)             \
{                               \
    return ((a > b) ? a : b);   \
}                               

TEMPLATE_MAX(int)

/********************************** Typedefs **********************************/
typedef struct public_transport PublicTransport;
typedef struct minibus Minibus;
typedef struct army_minibus ArmyMinibus;
typedef struct taxi Taxi;
typedef struct special_taxi SpecialTaxi;
typedef struct public_convoy PublicConvoy;

typedef void (*dtor)(PublicTransport* const this);
typedef void (*display)(PublicTransport* const this);

/****************** Forward declaration of virtual functions ******************/
void PublicTransportDtor(PublicTransport* const this);
void PublicTransportDisplay(PublicTransport* const this);

void MinibusDtor(Minibus* const this);
void MinibusDisplay(Minibus* const this);
void MinibusWash(Minibus* const this, int minutes);

void ArmyMinibusDtor(ArmyMinibus* const this);

void TaxiDtor(Taxi* const this);
void TaxiDisplay(Taxi* const this);

void SpecialTaxiDtor(SpecialTaxi* const this);
void SpecialTaxiDisplay(SpecialTaxi* const this);

void PublicConvoyDtor(PublicConvoy* const this);
void PublicConvoyDisplay(PublicConvoy* const this);

/******************************* Virtual tables *******************************/
typedef struct v_public_transport
{
    dtor m_dtor;
    display m_display;
}VTable_PT;

typedef struct v_minubus
{
    VTable_PT v_table;
    void (*wash)(Minibus* const this, int minutes);
}VMinibus;

/*********************** Global virtual table instances ***********************/
const VTable_PT g_v_base = {PublicTransportDtor, PublicTransportDisplay};
const VMinibus g_v_minibus = {{(dtor)MinibusDtor, (display)MinibusDisplay}
                                                                , MinibusWash};
const VMinibus g_v_army_minibus = {{(dtor)ArmyMinibusDtor
                                    , (display)MinibusDisplay}, MinibusWash};
const VTable_PT g_v_taxi = {(dtor)TaxiDtor, (display)TaxiDisplay};
const VTable_PT g_v_special_taxi 
                = {(dtor)SpecialTaxiDtor, (display)SpecialTaxiDisplay};
const VTable_PT g_v_public_convoy 
                = {(dtor)PublicConvoyDtor, (display)PublicConvoyDisplay};

/********************************** Structs ***********************************/
struct public_transport
{
    const VTable_PT* vptr;
    int m_license_plate;
};

struct minibus
{
    PublicTransport public_transport;
    int m_numSeats;
};

struct army_minibus
{
    Minibus minibus;
};

struct taxi
{
    PublicTransport public_transport;
};

struct special_taxi
{
    Taxi taxi;
};

struct public_convoy
{
    PublicTransport public_transport;
    PublicTransport* m_pt1;
    PublicTransport* m_pt2;
    Minibus m_m;
    Taxi m_t;
};

/*********************************** STATIC ***********************************/
static int g_count = 0;

static void PrintCount()
{
    printf("s_count: %d\n", g_count);
}

/****************************** PUBLIC_TRANSPORT *******************************/
void PublicTransportCtor(PublicTransport* const this)
{
    this->vptr = &g_v_base;

    this->m_license_plate = ++g_count;
    printf("PublicTransport::Ctor()%d\n", this->m_license_plate);
}

void PublicTransportCCtor(PublicTransport* const this
                                        , const PublicTransport* const other)
{
    UNUSED(other);
    
    this->vptr = &g_v_base;
    this->m_license_plate = ++g_count;
    printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
}

void PublicTransportDtor(PublicTransport* const this)
{
    this->vptr = &g_v_base;
    --g_count;
    printf("PublicTransport::Dtor()%d\n", this->m_license_plate);
}

void PublicTransportDisplay(PublicTransport* const this)
{
    printf("PublicTransport::display(): %d\n", this->m_license_plate);
}

int PublicTransportGet_ID(PublicTransport* const this)
{
    return this->m_license_plate;
}

/********************************** MINIBUS ***********************************/
void MinibusCtor(Minibus* const this)
{
    PublicTransportCtor((PublicTransport*)this);

    ((PublicTransport*)this)->vptr = (VTable_PT*)&g_v_minibus;

    this->m_numSeats = 20;
    printf("Minibus::Ctor()\n");
}

void MinibusCCtor(Minibus* const this, const Minibus* const other)
{
    PublicTransportCCtor((PublicTransport*)this, (PublicTransport*)other);
    
    ((PublicTransport*)this)->vptr = (VTable_PT*)&g_v_minibus;

    this->m_numSeats = other->m_numSeats;
    printf("Minibus::CCtor()\n");
}

void MinibusDtor(Minibus* const this)
{
    ((PublicTransport*)this)->vptr = (VTable_PT*)&g_v_minibus;
    printf("Minibus::Dtor()\n");
    
    PublicTransportDtor((PublicTransport*)this);
}

void MinibusDisplay(Minibus* const this)
{
    printf("Minibus::display() ID:%d"
                            , PublicTransportGet_ID((PublicTransport*)this));
    printf(" num seats:%d\n", this->m_numSeats);
}

void MinibusWash(Minibus* const this, int minutes)
{
    printf("Minibus::wash(%d) ID:%d\n"
                    , minutes, PublicTransportGet_ID((PublicTransport*)this));
}

/******************************** ARMY_MINIBUS ********************************/
void ArmyMinibusCtor(ArmyMinibus* const this)
{
    MinibusCtor((Minibus*)this);

    ((PublicTransport*)this)->vptr = (VTable_PT*)&g_v_army_minibus;

    printf("ArmyMinibus::Ctor()\n");
}

void ArmyMinibusCCtor(ArmyMinibus* const this, const ArmyMinibus* const other)
{   
    MinibusCCtor((Minibus*)this, (Minibus*)other);
    ((PublicTransport*)this)->vptr = (VTable_PT*)&g_v_army_minibus;

    printf("ArmyMinibus::CCtor()\n");
}

void ArmyMinibusDtor(ArmyMinibus* const this)
{
    ((PublicTransport*)this)->vptr = (VTable_PT*)&g_v_army_minibus;
    printf("ArmyMinibus::Dtor()\n");
    MinibusDtor((Minibus*)this);
}

/************************************ TAXI ************************************/
void TaxiCtor(Taxi* const this)
{
    PublicTransportCtor((PublicTransport*)this);

    ((PublicTransport*)this)->vptr = &g_v_taxi;

    printf("Taxi::Ctor()\n");
}

void TaxiCCtor(Taxi* const this, const Taxi* const other)
{
    PublicTransportCCtor((PublicTransport*)this, (PublicTransport*)other);
    
    ((PublicTransport*)this)->vptr = &g_v_taxi;
    
    printf("Taxi::CCtor()\n");
}

void TaxiDtor(Taxi* const this)
{
    ((PublicTransport*)this)->vptr = &g_v_taxi;
    printf("Taxi::Dtor()\n");
    PublicTransportDtor((PublicTransport*)this);
}

void TaxiDisplay(Taxi* const this)
{
    printf("Taxi::display() ID:%d\n"
                            , PublicTransportGet_ID((PublicTransport*)this));
}

/******************************** SPECIAL_TAXI ********************************/
void SpecialTaxiCtor(SpecialTaxi* const this)
{
    TaxiCtor((Taxi*)this);

    ((PublicTransport*)this)->vptr = &g_v_special_taxi;

    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxiCCtor(SpecialTaxi* const this, const SpecialTaxi* const other)
{
    TaxiCCtor((Taxi*)this, (Taxi*)other);
    
    ((PublicTransport*)this)->vptr = &g_v_special_taxi;
    
    printf("SpecialTaxi::CCtor()\n");
}

void SpecialTaxiDtor(SpecialTaxi* const this)
{
    ((PublicTransport*)this)->vptr = &g_v_special_taxi;
    printf("SpecialTaxi::Dtor()\n");

    TaxiDtor(&this->taxi);
}

void SpecialTaxiDisplay(SpecialTaxi* const this)
{
    printf("SpecialTaxi::display() ID:%d\n"
                        , PublicTransportGet_ID((PublicTransport*)this));
}


/******************************** PUBLIC_CONVOY *******************************/
void PublicConvoyCtor(PublicConvoy* const this)
{
    PublicTransportCtor((PublicTransport*)this);
    
    ((PublicTransport*)this)->vptr = &g_v_public_convoy;

    this->m_pt1 = malloc(sizeof(Minibus));

    MinibusCtor((Minibus*)this->m_pt1);
    
    this->m_pt2 = malloc(sizeof(Taxi));

    TaxiCtor((Taxi*)this->m_pt2);

    MinibusCtor(&this->m_m);
    TaxiCtor(&this->m_t);
}

void PublicConvoyCCtor(PublicConvoy* const this
                                            , const PublicConvoy* const other)
{
    PublicTransportCtor((PublicTransport*)this);

    ((PublicTransport*)this)->vptr = &g_v_public_convoy;

    this->m_pt1 = malloc(sizeof(Minibus));

    MinibusCCtor((Minibus*)this->m_pt1, (Minibus*)other->m_pt1);

    this->m_pt2 = malloc(sizeof(Taxi));

    TaxiCCtor((Taxi*)this->m_pt2, (Taxi*)other->m_pt2);

    MinibusCCtor(&this->m_m, &other->m_m);
    TaxiCCtor(&this->m_t, &other->m_t);
}

void PublicConvoyDtor(PublicConvoy* const this)
{
    ((PublicTransport*)this)->vptr = &g_v_public_convoy;
    
    this->m_pt1->vptr->m_dtor((PublicTransport*)this->m_pt1);
    free(this->m_pt1);

    this->m_pt2->vptr->m_dtor((PublicTransport*)this->m_pt2);
    free(this->m_pt2);

    TaxiDtor(&this->m_t);
    MinibusDtor(&this->m_m);

    PublicTransportDtor((PublicTransport*)this);
}

void PublicConvoyDisplay(PublicConvoy* const this)
{
    this->m_pt1->vptr->m_display((PublicTransport*)this->m_pt1);
    this->m_pt2->vptr->m_display((PublicTransport*)this->m_pt2);

    MinibusDisplay(&this->m_m);
    TaxiDisplay(&this->m_t);
}

/********************************** GLOBALS ***********************************/
void PublicTransportPrintInfo(PublicTransport* const a)
{
    a->vptr->m_display(a);
}

void MinibusPrintInfo(Minibus* const m)
{
    ((VMinibus*)m->public_transport.vptr)->wash(m, 3);
}

void PublicTransportPrintInfo_2(PublicTransport* p, int i) 
{
    Minibus ret;

    UNUSED(i);
    MinibusCtor(&ret);
    printf("print_info(int i)\n");
    MinibusDisplay(&ret);

    PublicTransportCCtor(p, &ret.public_transport);
    MinibusDtor(&ret);
}

void G_TaxiDisplay(Taxi* s)
{
    TaxiDisplay(s);
}

/************************************ MAIN ************************************/
int main()
{
    size_t i = 0;
    
    Minibus m;
    Minibus m2;
    Minibus arr2_m;
    Minibus arr3[4];

    PublicTransport tmp;
    PublicTransport arr2[3];
    PublicTransport* array[3];
    PublicConvoy* ts1;
    PublicConvoy* ts2;

    Taxi arr2_t;
    Taxi* arr4;
    Taxi tmp_st;

    SpecialTaxi st;

    ArmyMinibus* army_minibus;
    
    /* Lines 227 - 229 */
    MinibusCtor(&m);
    MinibusPrintInfo(&m);
    PublicTransportPrintInfo_2(&tmp, 3);
    PublicTransportDisplay(&tmp);
    PublicTransportDtor(&tmp);

    /* Lines 230 - 238 */
    array[0] = (PublicTransport*)malloc(sizeof(Minibus));

    MinibusCtor((Minibus*)array[0]);

    array[1] = (PublicTransport*)malloc(sizeof(Taxi));

    TaxiCtor((Taxi*)array[1]);

    array[2] = (PublicTransport*)malloc(sizeof(Minibus));

    MinibusCtor((Minibus*)array[2]);
    
    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr->m_display(array[i]);
    }

    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr->m_dtor(array[i]);
        free(array[i]);
    }

    /* Lines 240 - 244 */
    MinibusCtor(&arr2_m);
    PublicTransportCCtor(&arr2[0], (PublicTransport*)&arr2_m);

    TaxiCtor(&arr2_t);
    PublicTransportCCtor(&arr2[1], (PublicTransport*)&arr2_m);

    PublicTransportCtor(&arr2[2]);

    arr2_t.public_transport.vptr->m_dtor((PublicTransport*)&arr2_t);
    arr2_m.public_transport.vptr->m_dtor((PublicTransport*)&arr2_m);

    for (i = 0; i < 3; ++i)
    {
        arr2[i].vptr->m_display(arr2 + i);
    }

    PublicTransportPrintInfo(arr2);

    /* Line 246 */
    PrintCount();
    
    /* Lines 247 - 248 */
    MinibusCtor(&m2);
    PrintCount();

    /* Line 250 */
    for (i = 0; i < 4; ++i)
    {
        MinibusCtor(arr3 + i);
    }
    
    /* Lines 251 - 252 */
    arr4 = malloc(sizeof(Taxi) * 4 + sizeof(size_t));

    *(size_t*)arr4 = 4;

    for (i = 0; i < *(size_t*)arr4; ++i)
    {
        TaxiCtor(arr4 + sizeof(size_t) + i);
    }

    for (i = 0; i < *(size_t*)arr4; ++i)
    {
        arr4[sizeof(size_t) + 3 - i].public_transport.vptr->
                    m_dtor((PublicTransport*)(arr4 + sizeof(size_t) + 3 - i));
    }
    free(arr4);

    /* Lines 254 - 255 */
    printf("%d\n", MAX_int(1, 2));
    printf("%d\n", MAX_int(1, 2.0));

    /* Lines 256 - 257 */
    SpecialTaxiCtor(&st);

    TaxiCCtor(&tmp_st, &st.taxi);
    G_TaxiDisplay(&tmp_st);
    TaxiDtor(&tmp_st);

    /* PHASE B - START*/
    /* Lines 259 - 265 */
    ts1 = malloc(sizeof(PublicConvoy));

    PublicConvoyCtor(ts1);

    ts2 = malloc(sizeof(PublicConvoy));

    PublicConvoyCCtor(ts2, ts1);

    ts1->public_transport.vptr->m_display((PublicTransport*)ts1);
    ts2->public_transport.vptr->m_display((PublicTransport*)ts2);

    ts1->public_transport.vptr->m_dtor((PublicTransport*)ts1);
    free(ts1);

    ts2->public_transport.vptr->m_display((PublicTransport*)ts2);

    ts2->public_transport.vptr->m_dtor((PublicTransport*)ts2);
    free(ts2);
    /* PHASE B - END */
    
    /* Lines 267 - 270 */
    army_minibus = malloc(sizeof(ArmyMinibus));

    ArmyMinibusCtor(army_minibus);

    ((PublicTransport*)army_minibus)->vptr
                                ->m_display((PublicTransport*)army_minibus);
    ((VMinibus*)army_minibus->minibus.public_transport.vptr)->
                                                wash((Minibus*)army_minibus, 5);

    army_minibus->minibus.public_transport.vptr->
                            m_dtor((PublicTransport*)army_minibus);
    free(army_minibus);

    /* Line 272 - 273 */
    st.taxi.public_transport.vptr->m_dtor((PublicTransport*)&st.taxi);

    for (i = 0; i < 4; ++i)
    {
        arr3[3 - i].public_transport.vptr->
                                    m_dtor((PublicTransport*)(arr3 + 3 - i));
    }

    m2.public_transport.vptr->m_dtor((PublicTransport*)&m2);

    for (i = 3; i > 0; --i)
    {
        arr2[i - 1].vptr->m_dtor(arr2 + i - 1);
    }

    m.public_transport.vptr->m_dtor((PublicTransport*)&m);
    
    return 0;
}
