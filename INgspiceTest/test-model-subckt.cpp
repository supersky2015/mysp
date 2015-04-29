#include "stdafx.h"
#include <include/subckt.h>
#include <include/model.h>

void test_model_subckt()
{
	string npn = subckt_helper::Instance()->GetSubckt("generic-npn");
	string pnp = subckt_helper::Instance()->GetSubckt("generic-pnp");

	string npn1 = model_helper::Instance()->GetModel("generic-npn");
	string pnp1 = model_helper::Instance()->GetModel("generic-pnp");

}