open Belt;

type id = int;

type t = {
  id: id
};

type model = t;

module type Modelable = {
  type t;
  let toModel: (t => model);
};

module type T = {
  type t;
  let forIds: (list(t), list(id)) => list(t);
};

module Make =
       (Modelable: Modelable)
       : (T with type t := Modelable.t) => {
  let forIds = (models: list(Modelable.t), ids: list(id)) =>
    models |> List.keep(_, (model) => ids |> List.has(_, Modelable.toModel(model).id, (==)));
};
