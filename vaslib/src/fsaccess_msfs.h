/*! \file    msfs.fsaccess.h
    \author  Alexander Wemmer, alex@wemmer.at
*/

#ifndef MSFS_FSACCESS_H
#define MSFS_FSACCESS_H

#include "vlassert.h"
#include "config.h"

#include "fsuipc.h"
#include "fsaccess.h"

#include <cmath>
#include <QTime>


/////////////////////////////////////////////////////////////////////////////

typedef struct _MSFS_TCAS_DATA
{
    // 0 = empty, otherwise this is an FS-generated ID.
    //(Do not use this for anything other than checking if the slot is empty
    // or used it may be re-used for other things at a later date).
    qint32 id; 

    // 32-bit float, in degrees, -ve = South
    float lat;

    // 32-bit float, in degrees, -ve = West 
    float lon;

    // 32-bit float, in feet
    float alt;

    // 16-bits. Heading. Usual 360 degrees == 65536 format.
    // Note that this is degrees TRUE, not MAG
    qint16 hdg;

    // 16-bits. Knots Ground Speed
    qint16 gs;

    // 16-bits, signed feet per minute V/S
    qint16 vs;

    // Zero terminated string identifying the aircraft. By default this is:
    // Airline & Flight Number, or Tail number
    // For Tail number, if more than 14 chars you get the *LAST* 14
    // Airline name is truncated to allow whole flight number to be included
    char idATC[15];

    // Zero in FS2002, a status indication in FS2004�see list below.
    qint8 bState;

    // the COM1 frequency set in the AI aircraft`s radio. (0Xaabb as in 1aa.bb)
    qint16 com1;

    // Constructor
    _MSFS_TCAS_DATA() : id(0), lat(0.0), lon(0.0), alt(0), hdg(0), gs(0), vs(0), idATC{}, bState(0), com1(0) {};

    //convert
    void writeToTcasEntry(TcasEntry& entry)
    {
        entry.m_valid = true;
        entry.m_id = id;
        entry.m_position = Waypoint("TCAS", QString::null, lat, lon);
        //entry.m_altitude_ft = (int)(alt + 0.5);
        entry.m_altitude_ft = (int)ceil(alt);
        entry.m_true_heading = hdg;
        entry.m_groundspeed_kts = gs;
        entry.m_vs_fpm = vs;
    };

} MSFS_TCAS_DATA;

/////////////////////////////////////////////////////////////////////////////

//! MSFS Flightsim Access
class FSAccessMsFs : public FSAccess
{
    Q_OBJECT

public:
    //! Standard Constructor
    FSAccessMsFs(ConfigWidgetProvider* config_widget_provider, 
                 const QString& cfg_file, FlightStatus* flightstatus);

    //! Destructor
    ~FSAccessMsFs() override;

    Config* config() override { return &m_cfg; }

    //----- write access

    //! sets the NAV frequency, index 0 = NAV1, index 1 = NAV2
    bool setNavFrequency(int freq, uint nav_index) override;
    //! sets the ADF frequency, index 0 = ADF1, index 1 = ADF2
    bool setAdfFrequency(int freq, uint adf_index) override;
    //! sets the OBS angle, index 0 = NAV1, index 1 = NAV2
    bool setNavOBS(int degrees, uint nav_index) override;

    bool setAutothrustArm(bool armed) override;
    bool setAutothrustSpeedHold(bool on) override;
    bool setAutothrustMachHold(bool on) override;

    bool setFDOnOff(bool on) override;
    bool setAPOnOff(bool on) override;
    bool setAPHeading(double heading) override;
    bool setAPVs(int vs_ft_min) override;
    bool setAPAlt(unsigned int alt) override;
    bool setAPAirspeed(unsigned short speed_kts) override;
    bool setAPMach(double mach) override;
    bool setAPHeadingHold(bool on) override;
    bool setAPAltHold(bool on) override;

    bool setUTCTime(const QTime& utc_time) override;
    bool setUTCDate(const QDate& utc_date) override;

    bool freeThrottleAxes() override;
    bool setThrottle(double percent) override;

    bool setSBoxTransponder(bool on) override;
    bool setSBoxIdent() override;
    bool setFlaps(uint notch) override;

    bool freeControlAxes() override;
    bool freeAileronAxis() override;
    bool freeElevatorAxis() override;
    bool setAileron(double percent) override;
    bool setElevator(double percent) override;
    bool setElevatorTrimPercent(double percent) override;
    bool setElevatorTrimDegrees(double degrees) override;

    bool freeSpoilerAxis() override;
    bool setSpoiler(double percent) override;

    bool setNAV1Arm(bool on) override;
    bool setAPPArm(bool on) override;

    bool setAltimeterHpa(const double& hpa) override;

    bool setPushback(PushBack status) override;
    
    void writeFMCStatusToSim(const FMCStatusData& fmc_status_data) override;

protected slots:

    //! request data from the FS
    void slotRequestData();

    //! adapts the update timers
    void slotConfigChanged() { setupRefreshTimer(false); }

protected:

    //void run();
    bool checkLink();
    void setupRefreshTimer(bool pause_mode);

protected:

    //! MSFS fsaccess configuration
    Config m_cfg;

    //! MSFS access
    FSUIPC *m_fsuipc;

    //! used to trigger to read data from the flightsim
    QTimer m_refresh_timer;

    //! used to time tcas data fetches
    QTime m_tcas_refresh_timer;

    //! flaps raw value increase per flap notch
    uint m_flaps_inc_per_notch;

    short m_axes_disco_310a;
    char m_axes_disco_341a;

    QTime m_refresh_analyse_timer;

private:
    //! Hidden copy-constructor
    FSAccessMsFs(const FSAccessMsFs&);
    //! Hidden assignment operator
    const FSAccessMsFs& operator = (const FSAccessMsFs&);
};

#endif /* MSFS_FSACCESS_H */
