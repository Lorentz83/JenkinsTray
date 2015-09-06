#include "jenkinsjob.h"

#include <QDebug>

QDebug operator<<(QDebug dbg, const JobStatus &status){
    switch(status) {
    case JobStatus::SUCCESS:
        dbg.nospace() << "Success";
        break;
    case JobStatus::FAIL:
        dbg.nospace() << "Fail";
        break;
    case JobStatus::INSTABLE:
        dbg.nospace() << "Instable";
        break;
    default:
        dbg.nospace() << "Unknown";
    }
    return dbg.maybeSpace();
}


QDebug operator<<(QDebug dbg, const JenkinsJob &job){
    dbg.nospace() << job.name << " #" << job.buildNumber << " " << job.status;

    return dbg.maybeSpace();
}
