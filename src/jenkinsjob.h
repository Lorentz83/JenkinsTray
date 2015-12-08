#ifndef JENKINSJOB_H
#define JENKINSJOB_H

#include <QString>
#include <QList>

enum class JobStatus{UNKNOWN, SUCCESS, INSTABLE, FAILURE, RUNNING};
JobStatus parseJobStatus(const QString&);
QString  toQString(const JobStatus &);
JobStatus operator&& (JobStatus j1, JobStatus j2);

class JenkinsJob {
    bool _valid;
    QString _name;
    int _buildNumber;
    JobStatus _status;
    QString _url;

public:
    JenkinsJob() :
        _valid(false), _buildNumber(-1), _status(JobStatus::UNKNOWN) {
    }
    JenkinsJob(QString name, int buildNumber, JobStatus status, QString url) :
        _valid(true), _name(name), _buildNumber(buildNumber), _status(status), _url(url) {
    }

    int buildNumber() const { return _buildNumber; }
    JobStatus status() const {return _status; }
    QString url() const { return _url; }
    QString name() const { return _name; }
    bool isValid() const { return _valid; }
};


class JenkinsStatus {
    bool _valid;
    QList<JenkinsJob> _jobs;
    QString _error;

public:
    JenkinsStatus(const QString& error):   _valid(false), _error(error) { }
    JenkinsStatus(QList<JenkinsJob> jobs): _valid(true),  _jobs(jobs) { }

    bool isValid() const {
        return _valid;
    }

    const QList<JenkinsJob>& jobs() const {
        return _jobs;
    }

    const QString& errorMessage() const {
        return _error;
    }
};

class QDebug;
QDebug operator<<(QDebug dbg, const JenkinsJob &job);
QDebug operator<<(QDebug dbg, const JobStatus &status);

#endif // JENKINSJOB_H
