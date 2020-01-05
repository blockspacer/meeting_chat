#ifndef GALLERY_VIEW_H
#define GALLERY_VIEW_H

#include <QFrame>
#include <vector>
#include <algorithm>

namespace Ui {
class GalleryView;
}

class QGridLayout;

struct ContentView {
    int64_t id;
    QWidget* view;
};

class PermuteStrategy {
public:
    virtual ~PermuteStrategy() {}

    virtual void permute(std::vector<std::shared_ptr<ContentView>>& views) = 0;
};

class DefaultStrategy : public PermuteStrategy {
public:
    DefaultStrategy() {}

    void permute(std::vector<std::shared_ptr<ContentView>>& views) override
    {
        std::sort(views.begin(), views.end(), [](const auto& e1, const auto& e2){
            return e1->id < e2->id;
        });
    }
};

class GalleryView : public QFrame
{
    Q_OBJECT

    enum class Strategy : int {
      DEFAULT = 0
    };

public:
    explicit GalleryView(QWidget *parent = nullptr);

    ~GalleryView();

    void insertView(std::shared_ptr<ContentView> view);

    void removeView(int64_t id);

    QWidget* getView(int64_t id);

    void removeAll();

protected:
    void init();

    std::shared_ptr<PermuteStrategy> getPermuteStrategy(Strategy strategys);

    void permute(Strategy strategy = Strategy::DEFAULT);

    void permuteViews();

private:
    Ui::GalleryView *ui;

    QGridLayout* _gridLayout;

    std::vector<std::shared_ptr<ContentView>> _views;
};

#endif // GALLERY_VIEW_H
