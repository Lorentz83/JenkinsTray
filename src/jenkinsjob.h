#ifndef JENKINSJOB_H
#define JENKINSJOB_H

#include <QString>
#include <QList>

enum class JobStatus{UNKNOWN, SUCCESS, INSTABLE, FAILURE, RUNNING};
JobStatus parseJobStatus(const QString&);
QString  toQString(const JobStatus &);
JobStatus operator&& (JobStatus j1, JobStatus j2);

class JenkinsJob {
    bool valid_;
    QString name_;
    int buildNumber_;
    JobStatus status_;
    QString url_;

public:
    JenkinsJob() :
        valid_(false), buildNumber_(-1), status_(JobStatus::UNKNOWN) {
    }
    JenkinsJob(QString name, int buildNumber, JobStatus status, QString url) :
        valid_(true), name_(name), buildNumber_(buildNumber), status_(status), url_(url) {
    }

    int buildNumber() const { return buildNumber_; }
    JobStatus status() const {return status_; }
    QString url() const { return url_; }
    QString name() const { return name_; }
    bool isValid() const { return valid_; }
};


class JenkinsStatus {
    bool valid_;
    QList<JenkinsJob> jobs_;
    QString error_;

public:
    JenkinsStatus(const QString& error):   valid_(false), error_(error) { }
    JenkinsStatus(QList<JenkinsJob> jobs): valid_(true),  jobs_(jobs) { }

    bool isValid() const {
        return valid_;
    }

    const QList<JenkinsJob>& jobs() const {
        return jobs_;
    }

    const QString& errorMessage() const {
        return error_;
    }
};

class QDebug;
QDebug operator<<(QDebug dbg, const JenkinsJob &job);
QDebug operator<<(QDebug dbg, const JobStatus &status);

#endif // JENKINSJOB_H
