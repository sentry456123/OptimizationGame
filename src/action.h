#ifndef ACTION_H_
#define ACTION_H_

class Robot;

namespace action
{
    typedef void(*Function)(Robot&);

    void MoveForward(Robot&);
    void TurnRight(Robot&);
    void GoTo(Robot&);
    void GoToValue(Robot&);
    void ChangeValueTo(Robot&);
    void ChangeValueBy(Robot&);
    void MovePointerTo(Robot&);
    void MovePointerBy(Robot&);
    void CopyValueFrom(Robot&);
    void IfThen(Robot&);
    void IfNotThen(Robot&);
    void Exit(Robot&);

    const char* ToConstCharPtr(Function function, int arg);
}

#endif /* ACTION_H_ */
