#include <cstdio>
#include <string>
#include <vector>
#include "aeb/sim.hpp"
#include <fstream>

struct Scenario {
    std::string name;
    double ego_kmh;
    double lead_kmh;
    double gap_m;       //starting distance 
    double lead_decel;   // 0, expect CCRb (car to car rear break) where the lead car brakes
};

static bool braked(const aeb::SimResult& r){
    for (const aeb::Sample& p : r.trace){
        if(p.state >= aeb::Aebstate::PartialBrake)    return true;
    }
    return false;
}
int main(){
    std::vector<Scenario> scenarios = {
        // CCRs: lead stopped, start 4 s away -> gap = speed*4
        {"CCRs", 10, 0, aeb::kmh(10)*4, 0},
        {"CCRs", 20, 0, aeb::kmh(20)*4, 0},
        {"CCRs", 30, 0, aeb::kmh(30)*4, 0},
        {"CCRs", 40, 0, aeb::kmh(40)*4, 0},
        {"CCRs", 50, 0, aeb::kmh(50)*4, 0},
        {"CCRs", 60, 0, aeb::kmh(60)*4, 0},
        {"CCRs", 70, 0, aeb::kmh(70)*4, 0},
        {"CCRs", 80, 0, aeb::kmh(80)*4, 0},

        // CCRm: lead drives at 20 km/h -> gap = closing speed*4
        {"CCRm", 30, 20, aeb::kmh(30 - 20)*4, 0},
        {"CCRm", 50, 20, aeb::kmh(50 - 20)*4, 0},
        {"CCRm", 80, 20, aeb::kmh(80 - 20)*4, 0},

        // CCRb: both at 50 km/h, lead brakes suddenly (gap 12 or 40 m, -2 or -6 m/s^2)
        {"CCRb", 50, 50, 12, -2},
        {"CCRb", 50, 50, 12, -6},
        {"CCRb", 50, 50, 40, -2},
        {"CCRb", 50, 50, 40, -6},
    };
    
    std::ofstream csv("trace.csv");
    csv << "test,ego_kmh,lead_kmh,start_gap,lead_decel,t,ego_v,lead_v,gap,ttc,state\n";
    std::printf("%-6s %10s %10s %8s %7s  %s\n", "Test", "Ego", "Lead", "Gap", "Decel", "Result");
    for (const Scenario& sc: scenarios){   
        aeb::VehicleState ego {0, aeb::kmh(sc.ego_kmh), 0};
        aeb::VehicleState lead {sc.gap_m, aeb::kmh(sc.lead_kmh), sc.lead_decel};
        aeb::SimResult r = aeb::run(ego, lead);
        for (const aeb::Sample& p : r.trace) {
            csv << sc.name << ',' << sc.ego_kmh << ',' << sc.lead_kmh << ',' << sc.gap_m << ',' << sc.lead_decel << ','
                << p.t << ',' << p.ego_v << ',' << p.lead_v << ',' << p.gap << ',' << p.ttc << ','
                << static_cast<int>(p.state) << '\n';
        }
        if (r.collided)
            std::printf("%-6s %5.0f km/h %5.0f km/h %6.1f m %5.1f m/s2  FAIL (hit at %.1f km/h)\n",
                        sc.name.c_str(), sc.ego_kmh, sc.lead_kmh, sc.gap_m, sc.lead_decel, r.impact_speed * 3.6);
        else
            std::printf("%-6s %5.0f km/h %5.0f km/h %6.1f m %5.1f m/s2  PASS\n",
                        sc.name.c_str(), sc.ego_kmh, sc.lead_kmh, sc.gap_m, sc.lead_decel);
    }
    struct Setup { const char* name; aeb::RadarConfig radar; };
    std::vector<Setup> setups = {
        {"Perfect sensor",       {}},
        {"Noisy radar",          {0.2, 1.0, 42, false}},
        {"Noisy radar + Kalman", {0.2, 1.0, 42, true}},
    };

    std::printf("\n%-22s %10s %16s\n", "Sensor", "Avoided", "Phantom brakes");
    for (const Setup& su : setups) {
        // a) Euro NCAP: count the scenarios with no crash
        int avoided = 0;
        for (const Scenario& sc : scenarios) {
            aeb::VehicleState ego  {0, aeb::kmh(sc.ego_kmh), 0};
            aeb::VehicleState lead {sc.gap_m, aeb::kmh(sc.lead_kmh), sc.lead_decel};
            aeb::SimResult r = aeb::run(ego, lead, {}, 0.01, 30.0, su.radar);
            if (!r.collided) avoided++;
        }

        // b) queue traffic: 20 drives, each with different noise
        int phantom = 0;
        for (unsigned seed = 1; seed <= 20; seed++) {
            aeb::RadarConfig rc = su.radar;
            rc.seed = seed;
            aeb::VehicleState ego  {0.0, aeb::kmh(20), 0.0};
            aeb::VehicleState lead {5.0, aeb::kmh(20), 0.0};
            if (braked(aeb::run(ego, lead, {}, 0.01, 30.0, rc))) phantom++;
        }

        std::printf("%-22s %7d/15 %13d/20\n", su.name, avoided, phantom);
    }
    std::ofstream rcsv("radar.csv");
    rcsv << "t,true_closing,raw_closing,est_closing\n";
    aeb::VehicleState ego  {0.0, aeb::kmh(50), 0.0};
    aeb::VehicleState lead {12.0, aeb::kmh(50), -6.0};
    aeb::SimResult r = aeb::run(ego, lead, {}, 0.01, 30.0, {0.2, 1.0, 42, true});
    for (const aeb::Sample& p : r.trace)
        rcsv << p.t << ',' << p.ego_v -p.lead_v << ',' << p.raw_closing << ',' << p.est_closing << '\n';
    return 0;
}