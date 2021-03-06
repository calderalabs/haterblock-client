open Belt;

open MomentRe;

[%bs.raw {|require('./Settings.css')|}];

type action =
  | ToggleAutoRejectEnabled
  | ToggleEmailNotificationsEnabled;

type state = {user: UserData.User.t};

let component = ReasonReact.reducerComponent("Settings");

let make =
    (
      ~user: UserData.User.t,
      ~onUserUpdated: UserData.User.t => unit,
      _children,
    ) => {
  let save = (state, callback: Callback.t(unit, unit)) =>
    UserData.update(
      state.user,
      (_) => {
        callback(Success());
        onUserUpdated(state.user);
      },
    );
  {
    ...component,
    initialState: () => {user: user},
    reducer: (action, state) =>
      switch (action) {
      | ToggleAutoRejectEnabled =>
        ReasonReact.Update({
          user: {
            ...state.user,
            autoRejectEnabled: ! state.user.autoRejectEnabled,
          },
        })
      | ToggleEmailNotificationsEnabled =>
        ReasonReact.Update({
          user: {
            ...state.user,
            emailNotificationsEnabled: ! state.user.emailNotificationsEnabled,
          },
        })
      },
    render: self =>
      <div className="Settings">
        <Checkbox
          label="Auto Reject Hateful Comments"
          checked=self.state.user.autoRejectEnabled
          onChange=(() => self.ReasonReact.send(ToggleAutoRejectEnabled))
        />
        <Checkbox
          label="Email Notifications"
          checked=self.state.user.emailNotificationsEnabled
          onChange=(
            () => self.ReasonReact.send(ToggleEmailNotificationsEnabled)
          )
        />
        <div className="Settings__save">
          <AsyncButton className="Button--primary" onClick=(save(self.state))>
            (ReasonReact.stringToElement("Save"))
          </AsyncButton>
        </div>
      </div>,
  };
};
