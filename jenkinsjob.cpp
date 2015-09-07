#include "jenkinsjob.h"

#include <QDebug>

JobStatus operator&& (JobStatus j1, JobStatus j2) {
    return j1 > j2 ? j1:j2;
}

JobStatus parseJobStatus(const QString &str) {
    if (str == "stable") { //(back to normal)
        return JobStatus::SUCCESS;
    } else if (str.startsWith("broken")) { //(broken since this build) (broken since build #3515)
        return JobStatus::FAILURE;
    } else if (str == "back to normal") { //
        return JobStatus::INSTABLE;
    } else if (str == "?") { //
        return JobStatus::RUNNING;
    }
    return JobStatus::UNKNOWN;
}

QString  toQString(const JobStatus &status){
    switch(status) {
    case JobStatus::SUCCESS:
        return "Success";
    case JobStatus::FAILURE:
        return "Failure";
    case JobStatus::INSTABLE:
        return "Instable";
    case JobStatus::RUNNING:
        return "Running";
    default:
        return "Unknown";
    }
}

QDebug operator<<(QDebug dbg, const JobStatus &status){
    dbg.nospace() << toQString(status);
    return dbg.maybeSpace();
}


QDebug operator<<(QDebug dbg, const JenkinsJob &job){
    dbg.nospace() << job.name << " #" << job.buildNumber << " " << job.status;

    return dbg.maybeSpace();
}

