#ifndef JENKINSJOB_H
#define JENKINSJOB_H

#include <QString>


enum class JobStatus{SUCCESS, FAIL, INSTABLE};

class JenkinsJob {
public:
    JenkinsJob(){
        buildNumber = -1;
    }
    JenkinsJob(QString name, int buildNumber, JobStatus status) {
        this->name = name;
        this->buildNumber = buildNumber;
        this->status = status;
    }
    QString name;
    int buildNumber;
    JobStatus status;
};


class QDebug;
QDebug operator<<(QDebug dbg, const JenkinsJob &job);
QDebug operator<<(QDebug dbg, const JobStatus &status);

#endif // JENKINSJOB_H
