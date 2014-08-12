/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
struct TerminalDragger  : public Component
{
    TerminalDragger(Point<int> p)
    {
        setBounds (p.getX(), p.getY(), 30, 30);
        setRepaintsOnMouseActivity (true);
    }

    void paint (Graphics& g)
    {
        g.setColour (Colours::white.withAlpha (isMouseOverOrDragging() ? 0.9f : 0.5f));
        g.fillEllipse (getLocalBounds().reduced (3).toFloat());

        g.setColour (Colours::darkgreen);
        g.drawEllipse (getLocalBounds().reduced (3).toFloat(), 2.0f);
    }

    void resized() override
    {
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(), getHeight(), getWidth());
    }

    void moved() override
    {
        if (isMouseButtonDown())
            relativePos = getBounds().getCentre().toFloat() / Point<int> (getParentWidth(), getParentHeight()).toFloat();
    }

    void mouseDown(const MouseEvent& e) override
    {
        dragger.startDraggingComponent (this, e);
    }

    void mouseDrag(const MouseEvent& e) override
    {
        dragger.dragComponent (this, e, &constrainer);
    }

    Point<float> relativePos;

private:
    ComponentBoundsConstrainer constrainer;
    ComponentDragger dragger;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TerminalDragger);
};
//==============================================================================

OwnedArray<TerminalDragger> draggers;

MainContentComponent::MainContentComponent()
{
    setSize (500, 400);
}

MainContentComponent::~MainContentComponent()
{
    while (draggers.size() > 0) {
        TerminalDragger* d =
            static_cast<TerminalDragger*> (draggers.removeAndReturn(0));
        delete d;
    }
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffeeddff));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    for (int i = 0; i < draggers.size(); i++) {
        TerminalDragger* d = draggers[i];

        if (d != nullptr) {
            d->setCentrePosition(proportionOfWidth(d->relativePos.x),
                                 proportionOfHeight(d->relativePos.y));
        }
    }
}

void MainContentComponent::mouseDown(const MouseEvent& e)
{
    if (e.mods.isPopupMenu())
    {
        menuPosition = e.getPosition();
        showPopupMenu();
    }
}

void MainContentComponent::addTerminal()
{
    TerminalDragger* d = new TerminalDragger(menuPosition);
    draggers.add(d);
    addAndMakeVisible(d);
}

static void menuCallBack (
    const int result, MainContentComponent *owner)
{
    if (owner != nullptr)
    {
        switch (result)
        {
        case 1: owner->addTerminal(); break;
        default: break;
        }
    }
}

void MainContentComponent::showPopupMenu()
{
    PopupMenu m;
    m.setLookAndFeel(&getLookAndFeel());
    m.addItem(1, TRANS ("place terminal"), true, false);
    m.showMenuAsync (PopupMenu::Options(),
                     ModalCallbackFunction::forComponent(
                         menuCallBack, this));
}

void MainContentComponent::childBoundsChanged(Component* child)
{
    if (dynamic_cast<TerminalDragger*>(child) != nullptr) {
        update();
    }
}

void MainContentComponent::update()
{
}
