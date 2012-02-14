#include <limits.h>
#include <QVector>
#include <QDebug>
#include "barchartmodel_p.h"
#include "qbarcategory.h"
#include "qbarset.h"

QTCOMMERCIALCHART_BEGIN_NAMESPACE

BarChartModel::BarChartModel(QBarCategory *category, QObject *parent) :
    QObject(parent)
    ,mCategory(category)
{
}

BarChartModel::~BarChartModel()
{
    delete mCategory;
}


QBarCategory& BarChartModel::category()
{
    return *mCategory;
}

void BarChartModel::addBarSet(QBarSet *set)
{
    mDataModel.append(set);
}

void BarChartModel::removeBarSet(QBarSet *set)
{
    if (mDataModel.contains(set)) {
        mDataModel.removeOne(set);
        delete set;
    }
}

int BarChartModel::countSets()
{
    return mDataModel.count();
}

int BarChartModel::countCategories()
{
    int count(0);
    for (int i=0; i<mDataModel.count(); i++){
        // TODO: can we assume that all series have same number of values? If not. then which values are empty?
        int temp = mDataModel.at(i)->count();
        if (temp > count) {
            count = temp;
        }
    }
    return count;
}

int BarChartModel::countTotalItems()
{
    int total = mDataModel.count() * countCategories();
    return total;
}

qreal BarChartModel::min()
{
    Q_ASSERT(mDataModel.count() > 0);
    // TODO: make min and max members and update them when data changes.
    // This is slower since they are checked every time, even if data is same since previous call.
    qreal min = INT_MAX;

    for (int i=0; i <mDataModel.count(); i++) {
        int itemCount = mDataModel.at(i)->count();
        for (int j=0; j<itemCount; j++) {
            qreal temp = mDataModel.at(i)->valueAt(j);
            if (temp < min) {
                min = temp;
            }
        }
    }
    return min;
}

qreal BarChartModel::max()
{
    Q_ASSERT(mDataModel.count() > 0);

    // TODO: make min and max members and update them when data changes.
    // This is slower since they are checked every time, even if data is same since previous call.
    qreal max = INT_MIN;

    for (int i=0; i <mDataModel.count(); i++) {
        int itemCount = mDataModel.at(i)->count();
        for (int j=0; j<itemCount; j++) {
            qreal temp = mDataModel.at(i)->valueAt(j);
            if (temp > max) {
                max = temp;
            }
        }
    }

    return max;
}

qreal BarChartModel::valueAt(int set, int category)
{
    if ((set < 0) || (set >= mDataModel.count())) {
        // No set, no value.
        return 0;
    } else if ((category < 0) || (category >= mDataModel.at(set)->count())) {
        // No category, no value.
        return 0;
    }

    return mDataModel.at(set)->valueAt(category);
}

qreal BarChartModel::categorySum(int column)
{
    qreal sum(0);
    int count = mDataModel.count(); // Count rows

    for (int row = 0; row < count; row++) {
        if (column < mDataModel.at(row)->count()) {
            sum += mDataModel.at(row)->valueAt(column);
        }
    }
    return sum;
}

qreal BarChartModel::maxCategorySum()
{
    qreal max = INT_MIN;
    int count = countCategories();

    for (int col=0; col<count; col++) {
        qreal sum = categorySum(col);
        if (sum > max) {
            max = sum;
        }
    }
    return max;
}

QString BarChartModel::label(int category)
{
    return mCategory->label(category);
}

#include "moc_barchartmodel_p.cpp"

QTCOMMERCIALCHART_END_NAMESPACE
