#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc, free*/

/*** FORWARD DECLARATION ******************************************************/

typedef struct public_transport PublicTransport;
typedef struct minibus Minibus;
typedef struct army_minibus ArmyMinibus;
typedef struct taxi Taxi;
typedef struct special_taxi SpecialTaxi;
typedef struct public_convoy PublicConvoy;

typedef void (*dtor)(void* const this);
typedef void (*display)(void* const this);
typedef void (*wash)(Minibus* const this, int minutes);

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

/*** VIRTUAL TABLES ***********************************************************/

typedef struct vtable_public_transport
{
    dtor m_dtor;
    display m_display;
}V_Public;

typedef struct vtable_Minibus
{
    V_Public vptr;
    wash m_wash;
}V_Minibus;

const V_Public v_public_tansport = {(dtor)PublicTransportDtor
                                            , (display)PublicTransportDisplay};
const V_Minibus v_minibus = {{(dtor)MinibusDtor, (display)MinibusDisplay}
                                                                , MinibusWash};
const V_Minibus v_army_minibus = {{(dtor)ArmyMinibusDtor
                                    , (display)MinibusDisplay}, MinibusWash};
const V_Public v_taxi = {(dtor)TaxiDtor, (display)TaxiDisplay};
const V_Public v_special_taxi = {(dtor)SpecialTaxiDtor
                                                , (display)SpecialTaxiDisplay};
const V_Public v_public_convoy = {(dtor)PublicConvoyDtor
                                                , (display)PublicConvoyDisplay};

/*** STRUCTS ******************************************************************/

struct public_transport
{
    const V_Public* vptr;
    int license_plate;
};

struct minibus
{
    PublicTransport base;
    int num_seats;
};

struct army_minibus
{
    Minibus base;
};

struct taxi
{
    PublicTransport base;
};

struct special_taxi
{
    Taxi base;
};

struct public_convoy
{
    PublicTransport base;
    PublicTransport* m_pt1;
    PublicTransport* m_pt2;
    Minibus m_m;
    Taxi m_t;
};

/*** STATICS ******************************************************************/

static int count = 0;

static void PrintCount()
{
    printf("s_count: %d\n", count);
}

/*** Public_transport *********************************************************/

void PublicTransportCtor(PublicTransport* const this)
{
    this->vptr = &v_public_tansport;

    this->license_plate = ++count;
    printf("PublicTransport::Ctor()%d\n", this->license_plate);
}

void PublicTransportCCtor(PublicTransport* const this
                                            ,const PublicTransport* const other)
{
    (void)(other);
    this->vptr = &v_public_tansport;

    this->license_plate = ++count;
    printf("PublicTransport::CCtor() %d\n", this->license_plate);
}

void PublicTransportDtor(PublicTransport* const this)
{
    this->vptr = &v_public_tansport;
    --count;
    printf("PublicTransport::Dtor()%d\n", this->license_plate);
}

void PublicTransportDisplay(PublicTransport* const this)
{
    printf("PublicTransport::display(): %d\n", this->license_plate);
}

/*** Minibus ******************************************************************/

void MinibusCtor(Minibus* const this)
{
    PublicTransportCtor((PublicTransport*)this);
    ((PublicTransport*)this)->vptr = (V_Public*)&v_minibus;

    this->num_seats = 20;
    printf("Minibus::Ctor()\n");
}

void MinibusCCtor(Minibus* const this, const Minibus* const other)
{
    PublicTransportCCtor((PublicTransport*)this, (PublicTransport*)other);
    ((PublicTransport*)this)->vptr = (V_Public*)&v_minibus;

    this->num_seats = other->num_seats;
    printf("Minibus::CCtor()\n");
}

void MinibusDtor(Minibus* const this)
{
    ((PublicTransport*)this)->vptr = (V_Public*)&v_minibus;
    printf("Minibus::Dtor()\n");
    PublicTransportDtor((PublicTransport*)this);
}

void MinibusDisplay(Minibus* const this)
{
    printf("Minibus::display() ID:%d num seats:%d\n"
            , ((PublicTransport*)this)->license_plate, this->num_seats);
}

void MinibusWash(Minibus* const this, int minutes)
{
    printf("Minibus::wash(%d) ID:%d\n"
            , minutes, ((PublicTransport*)this)->license_plate);
}

/*** Army_minibus *************************************************************/

void ArmyMinibusCtor(ArmyMinibus* const this)
{
    MinibusCtor((Minibus*)this);
    ((PublicTransport*)this)->vptr = (V_Public*)&v_army_minibus;

    printf("ArmyMinibus::Ctor()\n");
}

void ArmyMinibusCCtor(ArmyMinibus* const this, const ArmyMinibus* const other)
{
    MinibusCCtor((Minibus*)this, (Minibus*)other);
    ((PublicTransport*)this)->vptr = (V_Public*)&v_army_minibus;

    printf("ArmyMinibus::CCtor()\n");
}

void ArmyMinibusDtor(ArmyMinibus* const this)
{
    ((PublicTransport*)this)->vptr = (V_Public*)&v_army_minibus;
    printf("ArmyMinibus::Dtor()\n");
    MinibusDtor((Minibus*)this);
}

/*** Taxi *********************************************************************/

void TaxiCtor(Taxi* const this)
{
    PublicTransportCtor((PublicTransport*)this);
    ((PublicTransport*)this)->vptr = &v_taxi;
    printf("Taxi::Ctor()\n");
}

void TaxiCCtor(Taxi* const this, const Taxi* const other)
{
    PublicTransportCCtor((PublicTransport*)this, (PublicTransport*)other);
    ((PublicTransport*)this)->vptr = &v_taxi;

    printf("Taxi::CCtor()\n");
}

void TaxiDtor(Taxi* const this)
{
    ((PublicTransport*)this)->vptr = &v_taxi;
    printf("Taxi::Dtor()\n");
    PublicTransportDtor((PublicTransport*)this);
}

void TaxiDisplay(Taxi* const this)
{
    printf("Taxi::display() ID:%d\n"
                                , ((PublicTransport*)this)->license_plate);
}

/*** Special_taxi *************************************************************/

void SpecialTaxiCtor(SpecialTaxi* const this)
{
    TaxiCtor((Taxi*)this);
    ((PublicTransport*)this)->vptr = &v_special_taxi;
    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxiCCtor(SpecialTaxi* const this, const SpecialTaxi* const other)
{
    TaxiCCtor((Taxi*)this, (Taxi*)other);
    ((PublicTransport*)this)->vptr = &v_special_taxi;
    printf("SpecialTaxi::CCtor()\n");
}

void SpecialTaxiDtor(SpecialTaxi* const this)
{
    ((PublicTransport*)this)->vptr = &v_special_taxi;
    printf("SpecialTaxi::Dtor()\n");
    TaxiDtor((Taxi*)this);
}

void SpecialTaxiDisplay(SpecialTaxi* const this)
{
    printf("SpecialTaxi::display() ID:%d\n"
                , ((PublicTransport*)this)->license_plate);
}

/*** Public_convoy ************************************************************/

void PublicConvoyCtor(PublicConvoy* const this)
{
    PublicTransportCtor((PublicTransport*)this);
    ((PublicTransport*)this)->vptr = &v_public_convoy;

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
    ((PublicTransport*)this)->vptr = &v_public_convoy;

    this->m_pt1 = malloc(sizeof(Minibus));
    MinibusCCtor((Minibus*)this->m_pt1, (Minibus*)other->m_pt1);

    this->m_pt2 = malloc(sizeof(Taxi));
    TaxiCCtor((Taxi*)this->m_pt2, (Taxi*)other->m_pt2);

    MinibusCCtor((Minibus*)&this->m_m, (Minibus*)&other->m_m);
    TaxiCCtor((Taxi*)&this->m_t, &other->m_t);
}

void PublicConvoyDtor(PublicConvoy* const this)
{
    ((PublicTransport*)this)->vptr = &v_public_convoy;
    
    this->m_pt1->vptr->m_dtor(this->m_pt1);
    free(this->m_pt1);
    this->m_pt2->vptr->m_dtor(this->m_pt2);
    free(this->m_pt2);

    TaxiDtor(&this->m_t);
    MinibusDtor(&this->m_m);
    PublicTransportDtor((PublicTransport*)this);
}

void PublicConvoyDisplay(PublicConvoy* const this)
{
    this->m_pt1->vptr->m_display(this->m_pt1);
    this->m_pt2->vptr->m_display(this->m_pt2);

    MinibusDisplay(&this->m_m);
    TaxiDisplay(&this->m_t);
}

/*** GLOBALS ******************************************************************/

void PrintInfoPublicTransport(PublicTransport* a)
{
    PublicTransportDisplay(a);
}

void PrintInfoMinibus(Minibus* m)
{
    MinibusWash(m, 3);
}

void PrintInfoInt(PublicTransport* tmp, int i)
{
    Minibus ret;
    
    (void)(i);
    MinibusCtor(&ret);
    printf("print_info(int i)\n");
    MinibusDisplay(&ret);

    PublicTransportCCtor(tmp, (PublicTransport*)&ret);
    MinibusDtor(&ret);
}

void Taxi_Display(Taxi* s)
{
    TaxiDisplay(s);
}

int MaxFunc(int a, int b)
{
    return ((a > b) * a + (b >= a) * b);
}

/*** Main *********************************************************************/

int main()
{
    int i = 0;
    
    PublicTransport tmp_ret;
    PublicTransport* array[3];
    PublicTransport arr2[3];
    Minibus m;
    Minibus m2;
    Minibus minibus_tmp;
    Minibus arr3[4];
    ArmyMinibus* armyminibus;
    Taxi tmp_st;
    Taxi taxi_tmp;
    Taxi* arr4;
    SpecialTaxi st;
    PublicConvoy* ts1;
    PublicConvoy* ts2;

    MinibusCtor(&m);
    PrintInfoMinibus(&m);
    PrintInfoInt(&tmp_ret, 3);
    PublicTransportDisplay(&tmp_ret);
    PublicTransportDtor(&tmp_ret);

    array[0] = malloc(sizeof(Minibus));
    MinibusCtor((Minibus*)array[0]);
    array[1] = malloc(sizeof(Taxi));
    TaxiCtor((Taxi*)array[1]);
    array[2] = malloc(sizeof(Minibus));
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
    

    MinibusCtor(&minibus_tmp);
    PublicTransportCCtor((arr2), (PublicTransport*)&minibus_tmp);
    TaxiCtor(&taxi_tmp);
    PublicTransportCCtor((arr2 + 1), (PublicTransport*)&taxi_tmp);
    PublicTransportCtor((arr2 + 2));
    ((PublicTransport*)&taxi_tmp)->vptr->m_dtor(&taxi_tmp);
    ((PublicTransport*)&minibus_tmp)->vptr->m_dtor(&minibus_tmp);

    for (i = 0; i < 3; ++i)
    {
        PublicTransportDisplay(arr2 + i); /* I think this is more correct 
                                            because there is an static binding
                                            during compilation time and then
                                            the function binds */
        /* arr2[i].vptr->m_display(arr2 + i); */
    }

    PrintInfoPublicTransport(arr2);
    
    PrintCount();
    MinibusCtor(&m2);
    PrintCount();

    for (i = 0; i < 4; ++i)
    {
        MinibusCtor(arr3 + i);
    }
    
    /* the metadata part - will have errors on vlg */
    arr4 = malloc(sizeof(size_t) + 4 * sizeof(Taxi));

    *(size_t*)arr4 = 4;

    for (i = 0; (size_t)i < *(size_t*)arr4; i++)
    {
        TaxiCtor((arr4 + sizeof(size_t) + i));
    }
    
    for (i = (int)(*(size_t*)arr4 - 1); i > -1 ; --i)
    {
        arr4[sizeof(size_t) + i].base.vptr->m_dtor(arr4 + sizeof(size_t) + i);
    }
    free(arr4);

    printf("%d\n", MaxFunc(1, 2));
    printf("%d\n", MaxFunc(1, 2.0f));
    SpecialTaxiCtor(&st);
    TaxiCCtor(&tmp_st, (Taxi*)&st);
    Taxi_Display(&tmp_st);
    TaxiDtor(&tmp_st);

    ts1 = malloc(sizeof(PublicConvoy));
    PublicConvoyCtor(ts1);
    ts2 = malloc(sizeof(PublicConvoy));
    PublicConvoyCCtor(ts2, ts1);
    ((PublicTransport*)ts1)->vptr->m_display(ts1);
    ((PublicTransport*)ts2)->vptr->m_display(ts2);
    ((PublicTransport*)ts1)->vptr->m_dtor(ts1);
    free(ts1);

    ((PublicTransport*)ts2)->vptr->m_display(ts2);
    ((PublicTransport*)ts2)->vptr->m_dtor(ts2);
    free(ts2);

    armyminibus = malloc(sizeof(ArmyMinibus));
    ArmyMinibusCtor(armyminibus);
    ((PublicTransport*)armyminibus)->vptr->m_display(armyminibus);
    ((V_Minibus*)armyminibus->base.base.vptr)->m_wash((Minibus*)armyminibus, 5);

    /* Dtors */
    ((Minibus*)armyminibus)->base.vptr->m_dtor(armyminibus);
    free(armyminibus);

    SpecialTaxiDtor(&st);
    for (i = 3; i > -1; --i)
    {
        MinibusDtor(arr3 + i);
    }

    MinibusDtor(&m2);

    for (i = 2; i > -1; --i)
    {
        PublicTransportDtor(arr2 + i); /* I think this is more correct 
                                            because there is an static binding
                                            during compilation time and then
                                            the function binds */
        /* arr2->vptr->m_dtor(arr2 + i); */
    }
    
    MinibusDtor(&m);

    return 0;
}
