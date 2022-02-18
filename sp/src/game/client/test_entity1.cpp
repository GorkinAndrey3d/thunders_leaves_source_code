#include "cbase.h"

class CTLTestEntityOne : public CBaseAnimating
{
public:
	DECLARE_CLASS(CTLTestEntityOne, CBaseAnimating);
	DECLARE_DATADESC();

	CTLTestEntityOne()
	{
		m_bActive = false;
	}

	void Spawn( void );
	void Precache(void);
	void MoveThink(void);
	void InputToggle(inputdata_t &inputData);
private:
	bool m_bActive;
	float m_flNextChangeTime;
};

LINK_ENTITY_TO_CLASS(my_model_entity, CTLTestEntityOne);

// Start of our data description for the class
BEGIN_DATADESC(CTLTestEntityOne)

// Save/restore our active state
DEFINE_FIELD(m_bActive, FIELD_BOOLEAN),
DEFINE_FIELD(m_flNextChangeTime, FIELD_TIME),

// Links our input name from Hammer to our input member function
DEFINE_INPUTFUNC(FIELD_VOID, "Toggle", InputToggle),

// Declare our think function
DEFINE_THINKFUNC(MoveThink),

END_DATADESC()