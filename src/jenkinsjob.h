#ifndef JENKINSJOB_H
#define JENKINSJOB_H

#include <QString>

enum class JobStatus{UNKNOWN, RUNNING, INSTABLE, SUCCESS, FAILURE};
JobStatus parseJobStatus(const QString&);
QString  toQString(const JobStatus &);
JobStatus operator&& (JobStatus j1, JobStatus j2);

class JenkinsJob {
public:
    JenkinsJob(){
        buildNumber = -1;
    }
    JenkinsJob(QString name, int buildNumber, JobStatus status, QString url) {
        this->name = name;
        this->buildNumber = buildNumber;
        this->status = status;
        this->url = url;
    }
    QString name;
    int buildNumber;
    JobStatus status;
    QString url;
};


class QDebug;
QDebug operator<<(QDebug dbg, const JenkinsJob &job);
QDebug operator<<(QDebug dbg, const JobStatus &status);

#endif // JENKINSJOB_H
