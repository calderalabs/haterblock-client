open Belt;

type state = {message: option(string)};

type action =
  | SetMessage(string);

let instance = ref(None);

let component = ReasonReact.reducerComponent("AppAlert");

let make = _children => {
  ...component,
  initialState: () => {message: None},
  didMount: self => {
    instance := Some(self);
    ReasonReact.NoUpdate;
  },
  willUnmount: _self => instance := None,
  reducer: (action, _state) =>
    switch (action) {
    | SetMessage(message) => ReasonReact.Update({message: Some(message)})
    },
  render: ({state}) =>
    switch (state.message) {
    | None => ReasonReact.nullElement
    | Some(message) =>
      <MessageBox type_="alert">
        (ReasonReact.stringToElement(message))
      </MessageBox>
    },
};
